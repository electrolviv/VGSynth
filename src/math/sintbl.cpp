#include "sintbl.hpp"
#include "mathdefs.h"

// TODO: optimization

static bool sintblDone = false;

// SINTable  1024 x 16-bit / 0..90 degree
static int16_t sintbl[1024];

// Fast calls reference table

int16_t sinvalue(uint8_t hh, uint16_t tblangle) { return 0; }
int16_t sawvalue(uint8_t hh, uint16_t tblangle) { return 0; }
int16_t meavalue(uint8_t hh, uint16_t tblangle) { return 0; }
int16_t isinvalue(uint8_t hh, uint16_t tblangle) { return 0; }
int16_t sinsh(uint8_t hh, uint16_t tblangle) { return 0; }
int16_t sindl(uint8_t hh, uint16_t tblangle) { return 0; }
int16_t sigr1(uint8_t hh, uint16_t tblangle) { return 0; }
int16_t sigr2(uint8_t hh, uint16_t tblangle) { return 0; }

typedef int16_t (*fastcall)(uint8_t hh, uint16_t tblangle);

//
fastcall tblforms[8] = {sinvalue, sawvalue, meavalue, isinvalue,
                        sinsh,    sindl,    sigr1,    sigr2};

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

/*
 *  Angle : 0...4095, 12-bit
 */
int16_t VHSigSrc::value(enSigForm form, uint16_t angle, uint16_t path1) {

  uint16_t roundangle = ROUNDANG(angle);
  uint16_t path2 = DEG360 - path1;                  // asym 128 is middle
  bool half = roundangle < path1;                   // half / full
  uint16_t starta = half ? 0 : 2048;                //
  uint16_t curpath = half ? path1 : path2;          //
  uint16_t posa = roundangle - (!half ? path1 : 0); //
  uint16_t safeangle = starta + APPROX(posa, (1 << 11), curpath); //
  uint8_t hh = (safeangle >> 10) & 3;                             // quarter
  uint16_t tblangle = safeangle & ROUNDMASK(10);                  // final angle

  int16_t r;

  if (form == eSigForm_SIN) {
    r = VHSigSrc::sinvalue(hh, tblangle);
  } else if (form == eSigForm_SAW) {
    r = VHSigSrc::sawvalue(hh, tblangle);
  } else if (form == eSigForm_MEA) {
    r = VHSigSrc::meavalue(hh, tblangle);
  } else if (form == eSigForm_ISIN) {
    r = VHSigSrc::isinvalue(hh, tblangle);
  } else if (form == eSigForm_SIN_SH) {
    r = VHSigSrc::sinsh(hh, tblangle);
  } else if (form == eSigForm_SIN_DL) {
    r = VHSigSrc::sindl(hh, tblangle);
  } else {
    r = 0;
  }

  return r;
}

int16_t VHSigSrc::sinvalue(uint8_t hh, uint16_t tblangle) {

  int16_t r;

  switch (hh) {
  case 0:
    r = SIN1V(tblangle);
    break;
  case 1:
    r = SIN2V(tblangle);
    break;
  case 2:
    r = SIN3V(tblangle);
    break;
  default:
    r = SIN4V(tblangle);
    break;
  }

  return r;
}

int16_t VHSigSrc::sawvalue(uint8_t hh, uint16_t tblangle) {

  int16_t r;

  switch (hh) {
  case 0:
    r = SAW1(tblangle);
    break;
  case 1:
    r = SAW2(tblangle);
    break;
  case 2:
    r = SAW3(tblangle);
    break;
  default:
    r = SAW4(tblangle);
    break;
  }

  return r;
}

int16_t VHSigSrc::meavalue(uint8_t hh, uint16_t tblangle) {
  bool half = hh >> 1;
  return half ? I16MAX : I16MIN;
}

int16_t VHSigSrc::isinvalue(uint8_t hh, uint16_t tblangle) {

  int16_t r;

  switch (hh) {
  case 0:
    r = SIN4U(tblangle);
    break;
  case 1:
    r = SIN3U(tblangle);
    break;
  case 2:
    r = SIN2D(tblangle);
    break;
  default:
    r = SIN1D(tblangle);
    break;
  }

  return r;
}

int16_t VHSigSrc::sinsh(uint8_t hh, uint16_t tblangle) {

  int16_t r;

  switch (hh) {
  case 0:
    r = SIN4U(tblangle);
    break;
  case 1:
    r = SIN2V(tblangle);
    break;
  case 2:
    r = SIN2D(tblangle);
    break;
  default:
    r = SIN4V(tblangle);
    break;
  }

  return r;
}

int16_t VHSigSrc::sindl(uint8_t hh, uint16_t tblangle) {

  int16_t r;

  switch (hh) {
  case 0:
    r = SIN1V(tblangle);
    break;
  case 1:
    r = SIN3U(tblangle);
    break;
  case 2:
    r = SIN3V(tblangle);
    break;
  default:
    r = SIN1D(tblangle);
    break;
  }

  return r;
}
