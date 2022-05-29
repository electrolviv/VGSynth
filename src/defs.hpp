#pragma once

#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>

#define PCM_DEVICE "default"

#define AUDIO_CHN_CNT 16

#define BUFF_CHUNK_SIZE (44100)

#define AUDIO_BUFF_SIZE (1024 * 4)

enum enGensigType {
  eGensigType_SIN = 0,
  eGensigType_SAW,
  eGensigType_MEA,
  eGensigType_SINI
};

#define INRANGE(V, F, T) ((V >= F) && (V <= T))

#define DBG_CHN_DUAL_EN 0
#define DBG_CHN_TRIA_EN 0
#define DBG_CHN_VOIOU_EN 0
#define DBG_CHN_VOIOD_EN 0
#define DBG_CHN_EXTRUD_EN 0
#define DBG_CHN_DISTOR_EN 0
#define DBG_CHN_FLANGE_EN 0
