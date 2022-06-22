#include "sintbl.hpp"
#include "mathdefs.h"

// TODO: optimization

static bool sintblDone = false;

// SINTable  1024 x 16-bit / 0..90 degree
static int16_t sintbl[1024];

#define FASTFUNC(name) static int16_t name(uint16_t tblangle)

FASTFUNC(sinvalue1) { return SIN1V(tblangle); }
FASTFUNC(sinvalue2) { return SIN2V(tblangle); }
FASTFUNC(sinvalue3) { return SIN3V(tblangle); }
FASTFUNC(sinvalue4) { return SIN4V(tblangle); }

FASTFUNC(sawvalue1) { return SAW1(tblangle); }
FASTFUNC(sawvalue2) { return SAW2(tblangle); }
FASTFUNC(sawvalue3) { return SAW3(tblangle); }
FASTFUNC(sawvalue4) { return SAW4(tblangle); }

FASTFUNC(meavalue1) { return MEA1; }
FASTFUNC(meavalue2) { return MEA2; }
FASTFUNC(meavalue3) { return MEA3; }
FASTFUNC(meavalue4) { return MEA4; }

FASTFUNC(sinival1) { return SIN4U(tblangle); }
FASTFUNC(sinival2) { return SIN3U(tblangle); }
FASTFUNC(sinival3) { return SIN2D(tblangle); }
FASTFUNC(sinival4) { return SIN1D(tblangle); }

FASTFUNC(sinsh1) { return SIN4U(tblangle); }
FASTFUNC(sinsh2) { return SIN2V(tblangle); }
FASTFUNC(sinsh3) { return SIN2D(tblangle); }
FASTFUNC(sinsh4) { return SIN4V(tblangle); }

FASTFUNC(sindl1) { return SIN1V(tblangle); }
FASTFUNC(sindl2) { return SIN3U(tblangle); }
FASTFUNC(sindl3) { return SIN3V(tblangle); }
FASTFUNC(sindl4) { return SIN1D(tblangle); }

FASTFUNC(sigxx1) { return 0; }
FASTFUNC(sigxx2) { return 0; }
FASTFUNC(sigxx3) { return 0; }
FASTFUNC(sigxx4) { return 0; }

FASTFUNC(sigyy1) { return 0; }
FASTFUNC(sigyy2) { return 0; }
FASTFUNC(sigyy3) { return 0; }
FASTFUNC(sigyy4) { return 0; }

/*
 *  Fast calls reference table
 */

typedef int16_t (*fastcall)(uint16_t tblangle);

fastcall tblforms[eSigForm_Last * 4] = {
    sinvalue1, sinvalue2, sinvalue3, sinvalue4, sawvalue1, sawvalue2, sawvalue3,
    sawvalue4, meavalue1, meavalue2, meavalue3, meavalue4, sinival1,  sinival2,
    sinival3,  sinival4,  sinsh1,    sinsh2,    sinsh3,    sinsh4,    sindl1,
    sindl2,    sindl3,    sindl4,    sigxx1,    sigxx2,    sigxx3,    sigxx4,
    sigyy1,    sigyy2,    sigyy3,    sigyy4};

void VHSigSrc::Init() {
  if (!sintblDone) {

    // Make SIN
    for (int i = 0; i < 1024; i++) {
      sintbl[i] = int((sin((i * PI * 2) / 4096) * 32767));
    }
    sintbl[1023] = 32767;
    sintblDone = true;
    }
}

uint16_t VHSigSrc::rescl(uint16_t angle, sSigScale *pscl) {

  uint16_t wdeg1 = pscl->DEGT - pscl->DEGF;
  // uint16_t wdeg2 = DEG360 - wdeg1;
  uint16_t wscl1 = pscl->SCLT - pscl->SCLF;
  // uint16_t wscl2 = DEG360 - wscl1;
  uint32_t r;

  if (angle < pscl->SCLF) {

    r = (angle * pscl->DEGF) / pscl->SCLF;

  } else if (angle < pscl->SCLT) {

    uint32_t a = angle - pscl->SCLF;
    r = pscl->DEGF + (a * wdeg1 / wscl1);

  } else {

    uint32_t a = angle - pscl->SCLT;
    uint16_t d1 = DEG360 - pscl->DEGT;
    uint16_t d2 = DEG360 - pscl->SCLT;
    r = pscl->DEGT + (a * d1 / d2);
  }

  return r;
}

//
// +++|+++|+++|+++
// ++|+++++++++|++

int16_t VHSigSrc::value(enSigForm form, uint16_t angle, sSigScale *pscl) {

  // Round on overflow
  uint16_t roundangle = ROUNDANG(angle);

  uint16_t finangle = (pscl == nullptr) ? roundangle : rescl(roundangle, pscl);

  // quarter (0,1,2,3) is 2 high bits
  uint8_t hh = (finangle >> 10) & 3;

  // final angle is lowest 10 bits
  uint16_t tblangle = finangle & ROUNDMASK(10);

  // ---------------------
  // Fast call table
  // ---------------------
  //   6  5  4  3  2  1  0
  //  F2 F1 F0 H1 H0 Q1 Q0
  // ---------------------
  int funcidx = (form << 2) | hh;

  return tblforms[funcidx](tblangle);
}
