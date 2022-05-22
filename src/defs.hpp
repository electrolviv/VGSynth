#pragma once

#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>

#define DEF_AUDIO_CHN_CNT 16
#define     BUFF_CHUNK_SIZE     (44100)

enum enGensigType {
  eGensigType_SIN = 0,
  eGensigType_SAW,
  eGensigType_MEA,
  eGensigType_SINI
};
