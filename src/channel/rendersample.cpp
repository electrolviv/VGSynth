#include "rendersample.hpp"
#include "abuffer.hpp"
#include "midi/vhmididefs.hpp"
#include "patterns.hpp"

#include "runtime/keyboard.hpp"
#include "runtime/sysclock.hpp"

// runtime channel
static uint8_t channelreadyidx = 0;

extern VHAudioChannel channels[AUDIO_CHN_CNT];

extern ABuffer abuffers[AUDIO_CHN_CNT];

static void ProcessKey(uint8_t k) {
  printf("Key %d\n", k);

  uint16_t freq = VHMIDI::pckeyfreq(k);

  channels[channelreadyidx].Press(freq);
#if DEF_MULTICHNS > 0
  channelreadyidx++;
#endif
  if (channelreadyidx >= AUDIO_CHN_CNT)
    channelreadyidx = 0;
}

void RenderAudioBuffer(int16_t *presult, int buflen) {

  uint8_t k;

  if (gKeyboard.get(&k)) {
    ProcessKey(k);
  }

  for (int j = 0; j < AUDIO_CHN_CNT; j++) {
    for (int i = 0; i < buflen; i++) {
      int16_t r = channels[j].Render();
      abuffers[j].buff[i] = r;
      presult[i] = (!j) ? r : presult[i] + r;
    }
  }

  //    // Next clk
  //    gRuntimeClock.ClkTick();
  //    if (!gRuntimeClock.runtime_bmp_clock) {
  //      patterns.NextPos();
  //    }

  //  for (int i = 0; i < buflen; i++) {
  //    int16_t r = 0;
  //    for (int z = 0; z < AUDIO_CHN_CNT; z++)
  //      r += channels[z].Render();
  //  }

  //  if(!gRuntimeClock.runtime_bmp_clock) {
  //      pattern_tick();
  //  }
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
        int16_t r = VHSigSrc::value(eSigForm_SIN, angle) / 4;

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
        int16_t r2 = VHSigSrc::value(eSigForm_SIN, angle2) / 4;

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
