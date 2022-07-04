#include <math.h>

#include "defs.hpp"
#include "sintbl.hpp"

#include "defs_sigsrc.h"

// TODO: optimization

/*!< SINTable  1024 x 16-bit / 0..90 degree */
int16_t sintbl[1024];

int16_t sinfunc(uint16_t angle) {

  int16_t r;

  // quarter (0,1,2,3) is 2 high bits
  uint8_t hh = (angle >> 10) & 3;

  // final angle is lowest 10 bits
  uint16_t qangle = angle & ROUNDMASK(10);

  switch (hh) {
  case 0:
    r = SIN1V(qangle);
    break;
  case 1:
    r = SIN2V(qangle);
    break;
  case 2:
    r = SIN3V(qangle);
    break;
  default: {
    r = SIN4V(qangle);
  }
  }

  return r;
}
