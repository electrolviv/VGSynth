#include "defs.hpp"
#include "midi/vhmididefs.hpp"
#include "rendersample.hpp"
#include "patterns.hpp"

#include "runtime/sysclock.hpp"
#include "runtime/keyboard.hpp"


extern VHAudioChannel      channels[DEF_AUDIO_CHN_CNT];
static uint8_t             channelreadyidx = 0;

/*
void pattern_tick() {

    uint16_t *  ppattern    = patterns.Get(patterns.runtimePatternNumber);
    uint16_t    pos         = patterns.runtimePatternPos;
    uint16_t    note        = ppattern[pos];

    if(note==0xFFFF)    { channels[0].Off(); }
    else if(note == 0)  {  } // Release
    else                { channels[0].Press(note); }

}
*/

void RenderAudioBuffer(int16_t *presult, int buflen) {

  uint8_t k;

  if (gKeyboard.get(&k)) {

    printf("Keyget %d\n", k);
    uint16_t freq = VHMIDI::pckeyfreq(k);

    channels[channelreadyidx].Press(freq);
    channelreadyidx++;
    if (channelreadyidx >= DEF_AUDIO_CHN_CNT)
      channelreadyidx = 0;
  }

  for (int i = 0; i < buflen; i++) {

    /*
    if(!gRuntimeClock.runtime_bmp_clock) {
        pattern_tick();
    }
    */

    int16_t r = 0;
    for (int z = 0; z < DEF_AUDIO_CHN_CNT; z++)
      r += channels[z].Render();

    presult[i] = r;

    // Next clk
    gRuntimeClock.ClkTick();
    if (!gRuntimeClock.runtime_bmp_clock) {
      patterns.NextPos();
    }
  }
}

void MakeBufferTest(int16_t *parr, uint16_t len, int16_t note) {


    uint16_t fangle  = ( ((uint32_t)65536) * note)      / 44100;
    uint16_t fangle2 = ( ((uint32_t)65536) * (note+3))  / 44100;

    int rangle = 0;
    int rangle2 = 0;

    for(int i=0; i < len;i++) {

        rangle += fangle;
        rangle &= 0xFFFF;

        uint16_t angle = rangle >> 6;
        int16_t r = VHSinTbl::GetValue(angle)/4;

        int dlev = 1*1024;
        if(r>0) r+=dlev; else r-=dlev;

        // Distortion
        int distlevp = 4*1024;
        int distlevm = distlevp*-1;

        if(r>distlevp)      { r = distlevp - (r-distlevp); }
        else if(r<distlevm) { r = distlevm + (distlevm-r); }

        // ========================================
        rangle2 += fangle2;
        rangle2 &= 0xFFFF;

        uint16_t angle2 = rangle2 >> 6;
        int16_t  r2 = VHSinTbl::GetValue(angle2)/4;

        int dlev2 = 1*1024;
        if(r2>0) r2+=dlev2; else r2-=dlev2;

        // Distortion
        int distlevp2 = 4*1024;
        int distlevm2 = distlevp2*-1;

        if(r2>distlevp2)      { r2 = distlevp2 - (r2-distlevp2); }
        else if(r2<distlevm2) { r2 = distlevm2 + (distlevm2-r2); }

        parr[i] = r + r2;
    }
}
