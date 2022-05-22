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

#define DBG_CHN_DUAL_EN 1
#define DBG_CHN_TRIA_EN 1
#define DBG_CHN_VOIOU_EN 1
#define DBG_CHN_VOIOD_EN 1
#define DBG_CHN_EXTRUD_EN 1
#define DBG_CHN_DISTOR_EN 1
