#pragma once

#include <stdint.h>

enum enSigForm {
  eSigForm_SIN = 0,
  eSigForm_SAW,
  eSigForm_MEA,
  eSigForm_ISIN,
  eSigForm_SIN_SH, /** Sharp sin mod */
  eSigForm_SIN_DL  /** Dull sin mod */
};

class VHSigSrc {
public:
  static void Init();

  // common
  static int16_t value(enSigForm form, uint16_t angle, uint16_t path1 = 2048);

  //
  static int16_t sinvalue(uint8_t hh, uint16_t tblangle);
  static int16_t sawvalue(uint8_t hh, uint16_t tblangle);
  static int16_t meavalue(uint8_t hh, uint16_t tblangle);
  static int16_t isinvalue(uint8_t hh, uint16_t tblangle);

  // sinus mod
  static int16_t sinsh(uint8_t hh, uint16_t tblangle);
  static int16_t sindl(uint8_t hh, uint16_t tblangle);

  // Reserved
  static int16_t cosvalue(uint8_t hh, uint16_t angle);
};
