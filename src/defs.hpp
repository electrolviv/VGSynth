#pragma once

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#ifndef PI
#define PI 3.14159265
#endif

#define ROUNDMASK(BITS) ((1 << BITS) - 1)
#define ROUNDANG(V) (V & ROUNDMASK(12))

#define DEG0 0
#define DEG360 4096

#define DEG180 (DEG360 >> 1)

#define DEG90 (DEG180 >> 1)
#define DEG270 (DEG180 + DEG90)

#define DEG45 (DEG90 >> 1)
#define DEG315 (DEG270 + DEG45)

#define DEG30 (DEG90 / 3)
#define DEG330 (DEG360 - DEG30)

// min max values
#define I16MAX (32767)
#define I16MIN (-32767)

#define INRANGE(V, F, T) ((V >= F) && (V <= T))
#define APPROX(POS, V, L) ((POS) * (V) / (L))

// sin 0`
#define SIN1V(V) (sintbl[V])

// sin 90`
#define SIN2V(V) (sintbl[1023 - (V)])

// sin 180`
#define SIN3V(V) (0 - sintbl[V])

// sin 270`
#define SIN4V(V) (0 - sintbl[1023 - (V)])

#define FREQMACRO1(NOTE) (((uint32_t)65536) * (NOTE)) / 44100

#define PCM_DEVICE "default"

#define AUDIO_CHN_CNT 16

#define BUFF_CHUNK_SIZE (44100)

#define AUDIO_BUFF_SIZE (1024 * 4)

//#define CHN_DEFCFG_FORM eSigForm_SIN
//#define CHN_DEFCFG_FORM eSigForm_MEA
//#define CHN_DEFCFG_FORM eSigForm_ISIN
//#define CHN_DEFCFG_FORM eSigForm_SIN_SH
//#define CHN_DEFCFG_FORM eSigForm_SIN_DL

// Channel switch
// #define DEF_MULTICHNS 0
#define DEF_MULTICHNS 1

// Default instrument configuration
// #define DEF_FORM 1
#define DEF_FORM 2

#if DEF_FORM == 1

#define DBG_CHN_DUAL_EN 0
#define DBG_CHN_TRIA_EN 0
#define DBG_CHN_VOIOU_EN 0
#define DBG_CHN_VOIOD_EN 0
#define DBG_CHN_EXTRUD_EN 0
#define DBG_CHN_DISTOR_EN 0
#define DBG_CHN_FLANGE_EN 0
#define DBG_CHN_SLIDE_EN 0

#define CHN_DEFCFG_FORM eSigForm_ISIN

// Default channel timing

#define CHN_DEFCFG_AMP_ATT 340
#define CHN_DEFCFG_AMP_SUS 256

#define CHN_DEFCFG_CLKS_ATT 240
#define CHN_DEFCFG_CLKS_DEC 300
#define CHN_DEFCFG_CLKS_SUS 14200
#define CHN_DEFCFG_CLKS_REL 64000

#define CHN_DEFCFG_ASYM_OFFSET 128

// 2048
#define CHN_DEFCFG_ASYM_VALUE 1024

#elif DEF_FORM == 2

#define DBG_CHN_DUAL_EN 0
#define DBG_CHN_TRIA_EN 0
#define DBG_CHN_VOIOU_EN 0
#define DBG_CHN_VOIOD_EN 0
#define DBG_CHN_EXTRUD_EN 0
#define DBG_CHN_DISTOR_EN 0
#define DBG_CHN_FLANGE_EN 0
#define DBG_CHN_SLIDE_EN 0

#define CHN_DEFCFG_FORM eSigForm_ISIN

// Default channel timing

#define CHN_DEFCFG_AMP_ATT 340
#define CHN_DEFCFG_AMP_SUS 300

#define CHN_DEFCFG_CLKS_ATT 240
#define CHN_DEFCFG_CLKS_DEC 2000
#define CHN_DEFCFG_CLKS_SUS 62200
#define CHN_DEFCFG_CLKS_REL 64000

#define CHN_DEFCFG_ASYM_OFFSET 128

// 2048
#define CHN_DEFCFG_ASYM_VALUE 1024

#else
#error("Source signal not defined")
#endif

extern int16_t sintbl[1024];
