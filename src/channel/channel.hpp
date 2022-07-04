#pragma once

#include "defs.hpp"
#include "sintbl.hpp"

#include "fltbnc.hpp"
#include "fltdec.hpp"

#include "effect_freq_drift.hpp"

class VHAudioChannel {

public:
  enum enKeyState {
    nKeyState_Off = 0,
    nKeyState_Attack,
    nKeyState_Hold,
    nKeyState_Decay,
    nKeyState_Sustain,
    nKeyState_Release
  };

  enum enVolume {
    nVolumeAttack = 0,
    nVolumeDecay,
    nVolumeSustain,
    nVolumeRelease,
    nVolumeOff
  };

  // ADSR
  struct stVolumeProps {
    uint16_t attackClocks;  //!< Attack
    uint16_t decayClocks;   //!< Decay
    uint16_t sustainClocks; //!< Sustain
    uint16_t releaseClocks; //!< Release
  };

  struct stVolumeRuntime {
    uint8_t state;
    uint16_t pos;
  };

  //
  EffectFrequencyDrift effectDrift;

  struct stBaseVoice {

    int freeFlag; // Channel Free or Used ?
    int keyState; // 0 - Off

    int frequency;
    int freqfangle; // Calculated from frequency
    int freqrangle; // Runtime angle

    int amplitudeatt; // Attack amplitude
    int amplitudesus; // Sustain amplitude
    int phase;        // 0 ... 1023  ( 512 = 50% )

    // 0 sin    ~~~~~~~~
    // 1 saw    /\/\/\/\
        // 2 mea    __``__``
    uint8_t sigtype;

    uint16_t asummetry_offs;
    uint16_t asymmetry_val;
  };

  struct stSubVoice {
    int subenabled;       // SubVoice Enabled
    int16_t subtype;      // 7FFE - Octave down, 7FFF - Octave up, +/- is diff
    int16_t subamplitude; // For background voices
    int freqfangle;       // Calculated from frequency
    int freqrangle;       // Runtime angle
  };

  struct stEffectDistortion {
    uint8_t enabled;
    uint8_t parity;
    int16_t value;
  };

  struct stEffectExtruder {
    uint8_t enabled;
    uint8_t parity;
    uint16_t level;
  };

  VHAudioChannel();

  ~VHAudioChannel();

  stVolumeProps volumePattern;
  stVolumeRuntime volumeRuntime = {0, 0};

  stBaseVoice voiceBase; // Main voice
  stSubVoice voiceDual;  // Near 1 voice
  stSubVoice voiceTrial; // Near 2 voice
  stSubVoice voiceOD;    // octave down
  stSubVoice voiceOU;    // octave up

  stEffectDistortion sEffDist; // Distortion ;)
  stEffectExtruder sEffExtruder;

  // Flange draft
  int16_t flangeVal[16] = {0};
  uint16_t flangePos;

  // Distortion

  // Note Released, Note Attack, Note Active
  // swing in active period

  // Subvoice 1
  // Subvoice 2
  // Sub-Base octave down
  // Sub-Trev octave up

  // Delay 1
  // Delay 2
  // Delay 3
  // Delay 4

  // Distortion or reflection level
  // Extrude level

  // RunTime
  int smpls_delay = 0; // Positive if delay activated

  void Press(int note);
  void Release();
  void Off();

  int16_t Render();

  int32_t GetAmpRuntime();

  void setSigForm(uint8_t sigform);

private:
  FLTBnc fltbnc;
  FLTDec fltdec;
};
