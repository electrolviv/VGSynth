#ifndef SIGSRC_HPP
#define SIGSRC_HPP

#include "defs.hpp"
#include "defs_sigsrc.h"

class VHSigSrc {
public:
  static void Init();

  /*
   *  Angle : 0`...89.9` = (0...4095), 12-bit
   *
   *  Return value : -int16 ... +int16
   */

  static int16_t value(enSigForm form, uint16_t angle,
                       sSigScale *pscl = nullptr);

private:
  // Reserved
  // static int16_t cosvalue(uint8_t hh, uint16_t angle);

  static uint16_t rescl(uint16_t angle, sSigScale *pscl);
};

#endif // SIGSRC_HPP
