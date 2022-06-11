#include "sintbl.hpp"
#include <math.h>

#define PI 3.14159265

#ifndef APPROX
#define APPROX(POS, V, L) ((POS) * (V) / (L))
#endif

#define ROUNDMASK ((1 << 12) - 1)
#define ROUNDA(V) (V & ROUNDMASK)

#define DEG180 2048
#define DEG360 4096

#define I16MAX (32767)
#define I16MIN (-32767)

// sin 0`
#define SIN1V(V) (sintbl[tblangle])

// sin 90`
#define SIN2V(V) (sintbl[1023 - V])

// sin 180`
#define SIN3V(V) (0 - sintbl[tblangle])

// sin 270`
#define SIN4V(V) (0 - sintbl[1023 - tblangle])

// sin 0` - 1
#define SIN1D(V) (I16MIN + sintbl[V])

// sin 90` - 1
#define SIN2D(V) (I16MIN + sintbl[1023 - V])

// sin 270` + 1
#define SIN3U(V) (I16MAX - sintbl[V])

// sin 270` + 1
#define SIN4U(V) (I16MAX - sintbl[1023 - V])

static bool sintblDone = false;

// SINTable  1024 x 16-bit / 0..90 degree
static int16_t      sintbl[1024];

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

  uint16_t roundangle = ROUNDA(angle);

  // asym 128 is middle
  uint16_t path2 = DEG360 - path1;

  // half / full
  bool half = roundangle < path1;

  uint16_t starta = half ? 0 : 2048;
  uint16_t posa = roundangle - (!half ? path1 : 0);
  uint16_t safeangle = starta + APPROX(posa, (1 << 11), half ? path1 : path2);
  uint8_t hh = (safeangle >> 10) & 3;
  uint16_t tblangle = safeangle & ((1 << 10) - 1);

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
    r = APPROX(tblangle, I16MAX, 1024);
    break;
  case 1:
    r = APPROX(1023 - tblangle, I16MAX, 1024);
    break;
  case 2:
    r = 0 - APPROX(tblangle, I16MAX, 1024);
    break;
  default:
    r = 0 - APPROX(1023 - tblangle, I16MAX, 1024);
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

int16_t VHSigSrc::sindl(uint8_t hh, uint16_t tblangle) { return 0; }
