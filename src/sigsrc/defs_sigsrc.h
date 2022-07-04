#pragma once

#include <stdint.h>

#define FASTFUNC(name) static int16_t name(uint16_t tblangle)

/*! Total 16 signals forms available 0..15 */
enum enSigForm {

  eSigForm_First = 0, /*!< Default enumeration mark */

  eSigForm_SIN = eSigForm_First, /*!< 0. SIN */
  eSigForm_SAW,                  /*!< 1. SAW */
  eSigForm_MEA,                  /*!< 2. MEA */
  eSigForm_ISIN,                 /*!< 3. SIN Inverted */
  eSigForm_SIN_SH,               /*!< 4. SIN sharp mod */
  eSigForm_SIN_DL,               /*!< 5. SIN dull  mod */
  eSigForm_SIN_DBL,              /*!< 6. Reserved */
  eSigForm_RESERVED2,            /*!< 7. Reserved */

  eSigForm_EXT0, /*!< Extended 0 */
  eSigForm_EXT1, /*!< Extended 1 */
  eSigForm_EXT2, /*!< Extended 2 */
  eSigForm_EXT3, /*!< Extended 3 */
  eSigForm_EXT4, /*!< Extended 4 */
  eSigForm_EXT5, /*!< Extended 5 */
  eSigForm_EXT6, /*!< Extended 6 */
  eSigForm_EXT7, /*!< Extended 7 */

  eSigForm_Last
};

struct sSigScale {
  uint16_t DEGF; //!< Origin angle from
  uint16_t DEGT; //!< Origin angle to
  uint16_t SCLF; //!< Scalled angle from
  uint16_t SCLT; //!< Scalled angle to
};
