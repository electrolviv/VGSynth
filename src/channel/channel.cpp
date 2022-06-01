#include "channel.hpp"
#include "math/sintbl.hpp"



static int slidespd = 0;

VHAudioChannel::VHAudioChannel() {

  //    volumePattern.attackClocks  = 1000;
  //    volumePattern.holdcClocks   = 1600;
  //    volumePattern.releaseClocks = 2000;

  volumePattern.attackClocks = CHN_DEFCFG_CLKS_ATT;
  volumePattern.decayClocks = CHN_DEFCFG_CLKS_DEC;
  volumePattern.sustainClocks = CHN_DEFCFG_CLKS_SUS;
  volumePattern.releaseClocks = CHN_DEFCFG_CLKS_REL;

  voiceBase.freeFlag = true;
  voiceBase.sigtype = CHN_DEFCFG_FORM;

  // voiceBase.sigtype = eSigForm_MEA;

  //  voiceBase.amplitude = 1024; // 1023 is 100%, 2048 is 200%
  // 1023 is 100%, 2048 is 200%
  voiceBase.amplitudeatt = CHN_DEFCFG_AMP_ATT;
  voiceBase.amplitudesus = CHN_DEFCFG_AMP_SUS;

  voiceBase.asummetry_offs = CHN_DEFCFG_ASYM_OFFSET;
  voiceBase.asymmetry_val = CHN_DEFCFG_ASYM_VALUE;

  voiceDual.subenabled = DBG_CHN_DUAL_EN;
  voiceDual.subtype = +4;
  voiceDual.subamplitude = 64;

  voiceTrial.subenabled = DBG_CHN_TRIA_EN;
  voiceTrial.subtype = -3;
  voiceTrial.subamplitude = 64;

  voiceOU.subenabled = DBG_CHN_VOIOU_EN;
  voiceOU.subtype = 0x7FFE;
  voiceOU.subamplitude = 64;

  voiceOD.subenabled = DBG_CHN_VOIOD_EN;
  voiceOD.subtype = 0x7FFE;
  voiceOD.subamplitude = 64;

  // TODO: Fix Extruder
  sEffExtruder.enabled = DBG_CHN_EXTRUD_EN;
  sEffExtruder.level = 128; // / 16;

  sEffDist.enabled = DBG_CHN_DISTOR_EN;
  sEffDist.value = (int16_t)(4 * 1024);

  flangePos = 0;
}

VHAudioChannel::~VHAudioChannel() {}

void VHAudioChannel::Press(int note) {
  volumeRuntime.state = 0;
  volumeRuntime.pos = 0;

  voiceBase.keyState = nKeyState_Attack;

  voiceBase.frequency = note;
  voiceBase.freqfangle = FREQMACRO1(note);
  voiceBase.freqrangle = 0;

  if (voiceDual.subenabled) {
    voiceDual.freqfangle = FREQMACRO1(note + voiceDual.subtype);
    voiceDual.freqrangle = 0;
  }

  if (voiceTrial.subenabled) {
    voiceTrial.freqfangle = FREQMACRO1(note + voiceTrial.subtype);
    voiceTrial.freqrangle = 0;
  }

  if (voiceOU.subenabled) {
    voiceOU.freqfangle = FREQMACRO1(note + voiceOU.subtype);
    voiceOU.freqrangle = 0;
  }

  if (voiceOD.subenabled) {
    voiceOD.freqfangle = FREQMACRO1(note + voiceOD.subtype);
    voiceOD.freqrangle = 0;
  }
}

void VHAudioChannel::Off() { voiceBase.keyState = nKeyState_Off; }

int16_t VHAudioChannel::Render() {

  if (voiceBase.keyState == nKeyState_Off)
    return 0;

  int32_t amplitudeRuntime = GetAmpRuntime();
  uint16_t adsr[] = {volumePattern.attackClocks, volumePattern.decayClocks,
                     volumePattern.sustainClocks, volumePattern.releaseClocks};
  bool last = (volumeRuntime.pos == adsr[volumeRuntime.state]);
  volumeRuntime.pos = last ? 0 : (volumeRuntime.pos + 1);
  volumeRuntime.state += last ? 1 : 0;

  if (volumeRuntime.state == nVolumeOff) {
    Off();
    return 0;
  }

  voiceBase.freqrangle += voiceBase.freqfangle;
  voiceBase.freqrangle &= 0xFFFF;

  enSigForm form = (enSigForm)voiceBase.sigtype;
  uint16_t angle = voiceBase.freqrangle >> 4;
  uint16_t asym = voiceBase.asymmetry_val;
  int32_t sigval = VHSigSrc::value(form, angle, asym);
  int16_t r = (int)(sigval * (amplitudeRuntime / 2)) / 1024;

  // Extruder
  bool pside = (r >= 0);
  if (sEffExtruder.enabled) {
    r += pside ? sEffExtruder.level : (0 - sEffExtruder.level);
  }

  // Activate distortion ?
  if (sEffDist.enabled) {

    int16_t over = pside ? (r - sEffDist.value) : ((0 - sEffDist.value) - r);

    if (over > 0) {
      over /= 8;
      r = pside ? (sEffDist.value - over)
                : ((0 - sEffDist.value) + over); // 2/3 todo
    }
  }

  // Subvoice render
  if (voiceDual.subenabled) {
    // voiceDual.freqrangle += voiceDual.freqfangle;
    // voiceDual.freqrangle &= 0xFFFF;
    // uint16_t langle = voiceDual.freqrangle >> 6;
    // uint16_t lamplitude = amplitudeRuntime * voiceDual.subamplitude >> 10;
    // r += GetSourceGenerator(voiceBase.sigtype, langle) * (lamplitude / 2) /
    // 1024;
  }

#if (DBG_CHN_FLANGE_EN > 0)
  // r += flangeVal/2;

  flangeVal[flangePos] = r;
  flangePos++;
  if (flangePos == 16)
    flangePos = 0;

  r += flangeVal[(flangePos + 2) & 15];
  r += flangeVal[(flangePos + 4) & 15];
  r += flangeVal[(flangePos + 6) & 15];

#endif

  // ---------------
  // Slide
  // ---------------
#if (DBG_CHN_SLIDE_EN > 0)
//#define SLIDE_D_SPD 8000
#define SLIDE_D_SPD 31000
#define SLIDE_D_LIMIT_FMIN 1 /* 375 */

  slidespd = (slidespd == SLIDE_D_SPD) ? 0 : slidespd + 1;

  if (!slidespd)
    if (voiceBase.freqfangle > SLIDE_D_LIMIT_FMIN)
      voiceBase.freqfangle--;
#endif

  return r * 2;
}

int32_t VHAudioChannel::GetAmpRuntime() {

  int32_t r;
  auto pos = volumeRuntime.pos;

  switch (volumeRuntime.state) {

  case nVolumeAttack: {

    auto wdth = volumePattern.attackClocks;
    auto hght = voiceBase.amplitudeatt;
    r = APPROX(pos, hght, wdth);
  } break;

  case nVolumeDecay: {
    auto wdth = volumePattern.decayClocks;
    auto hght = voiceBase.amplitudeatt - voiceBase.amplitudesus;
    r = voiceBase.amplitudeatt - APPROX(pos, hght, wdth);
  } break;

  case nVolumeSustain: {
    r = voiceBase.amplitudesus;
  } break;

  case nVolumeRelease: {
    auto wdth = volumePattern.releaseClocks;
    auto hght = voiceBase.amplitudesus;
    r = voiceBase.amplitudesus - APPROX(pos, hght, wdth);

  } break;

  default: {
    r = 0;
  }
  }

  return r;
}
