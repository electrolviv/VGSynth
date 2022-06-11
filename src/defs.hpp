#pragma once

#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>

#define INRANGE(V, F, T) ((V >= F) && (V <= T))
#define APPROX(POS, V, L) ((POS) * (V) / (L))

#define FREQMACRO1(NOTE) (((uint32_t)65536) * (NOTE)) / 44100

#define PCM_DEVICE "default"

#define AUDIO_CHN_CNT 16

#define BUFF_CHUNK_SIZE (44100)

#define AUDIO_BUFF_SIZE (1024 * 4)

#define DBG_CHN_DUAL_EN 0
#define DBG_CHN_TRIA_EN 0
#define DBG_CHN_VOIOU_EN 0
#define DBG_CHN_VOIOD_EN 0
#define DBG_CHN_EXTRUD_EN 0
#define DBG_CHN_DISTOR_EN 0
#define DBG_CHN_FLANGE_EN 0
#define DBG_CHN_SLIDE_EN 0

//#define CHN_DEFCFG_FORM eSigForm_SIN
//#define CHN_DEFCFG_FORM eSigForm_SAW
//#define CHN_DEFCFG_FORM eSigForm_MEA
//#define CHN_DEFCFG_FORM eSigForm_ISIN
#define CHN_DEFCFG_FORM eSigForm_SIN_SH
//#define CHN_DEFCFG_FORM eSigForm_SIN_DL

// Default channel timing

#define CHN_DEFCFG_AMP_ATT 340
#define CHN_DEFCFG_AMP_SUS 256

#define CHN_DEFCFG_CLKS_ATT 240
#define CHN_DEFCFG_CLKS_DEC 300
#define CHN_DEFCFG_CLKS_SUS 14200
#define CHN_DEFCFG_CLKS_REL 64000

// TODO ?
#define CHN_DEFCFG_ASYM_OFFSET 1024

#define CHN_DEFCFG_ASYM_VALUE 2048
