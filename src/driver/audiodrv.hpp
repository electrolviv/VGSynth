#pragma once

#include <stdint.h>

extern "C" {
#include <alsa/asoundef.h>
#include <alsa/asoundlib.h>
#include <pthread.h>
};

class VHAudioDriver {

public:
  VHAudioDriver();
  ~VHAudioDriver();

  bool Init();
  void Deinit();

  bool IsDataNeed();
  void ResetFlagDataNeed();
  int16_t *GetBufferPtr(int *plen);

  void Thread();

private:
  bool flagInit;
  bool flagExit;
  bool flagFillData;
  int buffidx;

  static const int channels = 1;
  static const int rate = 44100;

  int pcm;
  snd_pcm_uframes_t frames;
  snd_pcm_t *pcm_handle;
  snd_pcm_hw_params_t *params;

  int buffsize;
  int16_t buff1[1024 * 4];
  int16_t buff2[1024 * 4];

  // thread id
  pthread_t thr;
};

extern VHAudioDriver globalAudioDriver;
