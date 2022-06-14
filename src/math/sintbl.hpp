#pragma once

#include <stdint.h>

enum enSigForm {
  eSigForm_First = 0,
  eSigForm_SIN = eSigForm_First,
  eSigForm_SAW,       /** */
  eSigForm_MEA,       /** */
  eSigForm_ISIN,      /** Inverted sin */
  eSigForm_SIN_SH,    /** Sharp sin mod */
  eSigForm_SIN_DL,    /** Dull sin mod */
  eSigForm_RESERVED1, /** Reserved */
  eSigForm_RESERVED2, /** Reserved */
  eSigForm_Last
};

class VHSigSrc {
public:
  static void Init();

  /*
   *  Angle : 0`...89.9` = (0...4095), 12-bit
   *
   *  Return value : -int16 ... +int16
   */

  static int16_t value(enSigForm form, uint16_t angle, uint16_t path1 = 2048);

private:
  // Reserved
  // static int16_t cosvalue(uint8_t hh, uint16_t angle);
};
