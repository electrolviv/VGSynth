#include "channel.hpp"
#include "math/sintbl.hpp"



static int slidespd = 0;

VHAudioChannel::VHAudioChannel() {

  //    volumePattern.attackClocks  = 1000;
  //    volumePattern.holdcClocks   = 1600;
  //    volumePattern.releaseClocks = 2000;

  volumePattern.attackClocks = 240;
  volumePattern.decayClocks = 100;
  volumePattern.sustainClocks = 1200;
  volumePattern.releaseClocks = 64000;

  voiceBase.freeFlag = true;
  voiceBase.sigtype = eGensigType_SINI;
  //  voiceBase.amplitude = 1024; // 1023 is 100%, 2048 is 200%
  voiceBase.amplitudeatt = 512; // 1023 is 100%, 2048 is 200%
  voiceBase.amplitudesus = 256;

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

  sEffExtruder.enabled = DBG_CHN_EXTRUD_EN;
  sEffExtruder.level = 1024 * 1 / 16;

  sEffDist.enabled = DBG_CHN_DISTOR_EN;
  sEffDist.value = (int16_t)(4 * 1024);

  flangePos = 0;
}

VHAudioChannel::~VHAudioChannel() {}

#define FREQMACRO1(NOTE) (((uint32_t)65536) * (NOTE)) / 44100

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

  uint16_t angle = voiceBase.freqrangle >> 6;
  int16_t r = (int)(GetSourceGenerator(voiceBase.sigtype, angle) *
                    (amplitudeRuntime / 2)) /
              1024;
  bool pside = (r >= 0);

  if (sEffExtruder.enabled) {
    r += pside ? sEffExtruder.level : (0 - sEffExtruder.level);
  }

  if (sEffDist.enabled) {

    int16_t over = pside ? (r - sEffDist.value) : ((0 - sEffDist.value) - r);

    // Activate distortion ?
    if (over > 0) {
      over /= 8;
      r = pside ? (sEffDist.value - over)
                : ((0 - sEffDist.value) + over); // 2/3 todo
    }
  }

  // Subvoice render
  if (voiceDual.subenabled) {
    voiceDual.freqrangle += voiceDual.freqfangle;
    voiceDual.freqrangle &= 0xFFFF;
    uint16_t langle = voiceDual.freqrangle >> 6;
    uint16_t lamplitude = amplitudeRuntime * voiceDual.subamplitude >> 10;
    r +=
        GetSourceGenerator(voiceBase.sigtype, langle) * (lamplitude / 2) / 1024;
  }

#if defined(DBG_CHN_FLANGE_EN)
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
#define SLIDE_D_SPD 8000
#define SLIDE_D_LIMIT_FMIN 1 /* 375 */

  slidespd = (slidespd == SLIDE_D_SPD) ? 0 : slidespd + 1;

  if (!slidespd)
    if (voiceBase.freqfangle > SLIDE_D_LIMIT_FMIN)
      voiceBase.freqfangle--;

  return r / 4;
}

int32_t VHAudioChannel::GetAmpRuntime() {

  int32_t r;

  switch (volumeRuntime.state) {

  case nVolumeAttack: {
    r = volumeRuntime.pos * voiceBase.amplitudeatt / volumePattern.attackClocks;
  } break;

  case nVolumeDecay: {
    int32_t delta = voiceBase.amplitudeatt - voiceBase.amplitudesus;
    r = voiceBase.amplitudeatt -
        (volumeRuntime.pos * delta / volumePattern.decayClocks);
  } break;

  case nVolumeSustain: {
    r = voiceBase.amplitudesus;

  } break;

  case nVolumeRelease: {
    r = voiceBase.amplitudesus - (volumeRuntime.pos * voiceBase.amplitudesus /
                                  volumePattern.releaseClocks);

  } break;

  default: {
    r = 0;
  }
  }

  return r;
}

int32_t VHAudioChannel::GetSourceGenerator(int sigtype, uint16_t angle) {
  uint16_t safeangle = angle & 1023;
  uint16_t safeanglei = (angle + 256) & 1023;

  if (sigtype == eGensigType_SIN) {
    return VHSinTbl::GetValue(safeangle);
  } else if (sigtype == eGensigType_SAW) {
    return -32767 + (safeangle * 64);
  } else if (sigtype == eGensigType_MEA) {
    return (safeangle < 512) ? 32000 : -32000;
  } else if (sigtype == eGensigType_SINI) {
    int16_t s = VHSinTbl::GetValue(safeanglei);
    int16_t sm = (s > 0) ? s : (0 - s);
    if (safeangle < 512) {
      return 32767 - sm;
    } else {
      return 0 - (32767 - sm);
    }
  } else {
    return VHSinTbl::GetValue(safeangle);
  }

  return 0;
}
