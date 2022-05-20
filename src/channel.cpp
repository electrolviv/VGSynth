#include "channel.hpp"
#include "math/sintbl.hpp"

enum enGensigType {
    eGensigType_SIN = 0,
    eGensigType_SAW,
    eGensigType_MEA,
    eGensigType_SINI
};


static int slidespd = 0;

VHAudioChannel::VHAudioChannel()    {


//    volumePattern.attackClocks  = 1000;
//    volumePattern.holdcClocks   = 1600;
//    volumePattern.releaseClocks = 2000;

    volumePattern.attackClocks  = 400;
    volumePattern.holdcClocks   = 200;
    volumePattern.releaseClocks = 13000;

    voiceBase.freeFlag        = true;
    voiceBase.sigtype         = eGensigType_SINI;
    voiceBase.amplitude       = 1024; // 1023 is 100%, 2048 is 200%

    voiceDual.subenabled      = 1;
    voiceDual.subtype         = +4;
    voiceTrial.subamplitude   = 256;

    voiceTrial.subenabled     = 1;
    voiceTrial.subtype        = -2;
    voiceTrial.subamplitude   = 256;

    voiceOU.subenabled        = 1;
    voiceOU.subtype           = 0x7FFE;
    voiceOU.subamplitude      = 256;

    voiceOD.subenabled        = 1;
    voiceOD.subtype           = 0x7FFE;
    voiceOD.subamplitude      = 256;

    sEffExtruder.enabled      = 1;
    sEffExtruder.level        = 1024*1/16;

    sEffDist.enabled          = 1;
    sEffDist.value            = (int16_t)( 4*1024);

    flangePos = 0;

}

VHAudioChannel::~VHAudioChannel()   {
}

void VHAudioChannel::Press(int note)
{
    volumeRuntime.state = 0;
    volumeRuntime.pos   = 0;

    voiceBase.keyState  = nKeyState_Attack;

    voiceBase.frequency = note;
    voiceBase.freqfangle = ( ((uint32_t)65536) * note) / 44100;
    voiceBase.freqrangle = 0;

    if(voiceDual.subenabled) {
        voiceDual.freqfangle = ( ((uint32_t)65536) * (note+voiceDual.subtype)) / 44100;
        voiceDual.freqrangle = 0;
    }

}

void VHAudioChannel::Off()
{
    voiceBase.keyState = nKeyState_Off;
}

int16_t VHAudioChannel::Render()
{
    if(voiceBase.keyState == nKeyState_Off)
        return 0;

    // Amplitude
    int32_t amplitudeRuntime;

    if(volumeRuntime.state==0) {
        amplitudeRuntime = volumeRuntime.pos * voiceBase.amplitude / volumePattern.attackClocks;
        volumeRuntime.pos++;
        if(volumeRuntime.pos == volumePattern.attackClocks) { volumeRuntime.pos = 0; volumeRuntime.state++; }

    } else if(volumeRuntime.state==1) {
        amplitudeRuntime = voiceBase.amplitude;
        volumeRuntime.pos++;
        if(volumeRuntime.pos == volumePattern.holdcClocks) { volumeRuntime.pos = 0; volumeRuntime.state++; }

    } else if(volumeRuntime.state==2) {
        amplitudeRuntime = voiceBase.amplitude - (volumeRuntime.pos * voiceBase.amplitude / volumePattern.releaseClocks);
        volumeRuntime.pos++;
        if(volumeRuntime.pos == volumePattern.releaseClocks) { volumeRuntime.pos = 0; volumeRuntime.state++; Off(); }

    }  else {
        return 0;
    }

    voiceBase.freqrangle += voiceBase.freqfangle;
    voiceBase.freqrangle &= 0xFFFF;

    uint16_t    angle = voiceBase.freqrangle >> 6;
    int16_t     r = (int)(GetSourceGenerator(voiceBase.sigtype, angle) * (amplitudeRuntime/2))/1024;
    bool        pside = (r>=0);

    if(sEffExtruder.enabled) {
        r += pside ? sEffExtruder.level : (0-sEffExtruder.level);
    }

    if(sEffDist.enabled) {

        int16_t over = pside ? (r-sEffDist.value): ( (0-sEffDist.value)-r ) ;

        // Activate distortion ?
        if(over>0) {
            over/=8;
            r = pside ?  (sEffDist.value - over) : ((0-sEffDist.value)+over); // 2/3 todo
        }

    }

    // Subvoice render
    if(voiceDual.subenabled) {
        voiceDual.freqrangle += voiceDual.freqfangle;
        voiceDual.freqrangle &= 0xFFFF;
        uint16_t angle = voiceDual.freqrangle >> 6;
        uint16_t amplitude = amplitudeRuntime * voiceDual.subamplitude >> 10;
        r += GetSourceGenerator(voiceBase.sigtype, angle) * (amplitude/2) / 1024;
    }

    // r += flangeVal/2;
    flangeVal[flangePos] = r;
    flangePos++;
    if(flangePos==16) flangePos = 0;

    r += flangeVal[(flangePos+2)&15];
    r += flangeVal[(flangePos+4)&15];
    r += flangeVal[(flangePos+6)&15];

    // ---------------
    // Slide
    // ---------------
//    slidespd++;
//    if(slidespd==1200) slidespd = 0;

//    if(!slidespd)
//        if(voiceBase.freqfangle>375)
//            voiceBase.freqfangle--;

    return r/2;
}

int32_t VHAudioChannel::GetSourceGenerator(int sigtype, uint16_t angle)
{
    uint16_t safeangle  = angle & 1023;
    uint16_t safeanglei = (angle + 256) & 1023;

    if(sigtype == eGensigType_SIN)  { return VHSinTbl::GetValue(safeangle); }
    else
    if(sigtype == eGensigType_SAW)  { return -32767 + (safeangle * 64); }
    else
    if(sigtype == eGensigType_MEA)  { return (safeangle<512) ? 32000:-32000; }
    else
    if(sigtype == eGensigType_SINI) {
        int16_t s  = VHSinTbl::GetValue(safeanglei);
        int16_t sm = (s>0)? s : (0-s);
        if(safeangle<512) { return 32767-sm; } else { return 0 - (32767-sm); }
    }
    else
    {
        return VHSinTbl::GetValue(safeangle);
    }

    return 0;
}
