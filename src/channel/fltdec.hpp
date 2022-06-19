#ifndef FLTDEC_HPP
#define FLTDEC_HPP

#include <stdint.h>

class FLTDec {

public:
  FLTDec() : prev(0), deltamax(4000) {}

  int16_t ins(int16_t val) {

    bool positive = val > prev;

    uint16_t delta = positive ? (val - prev) : (prev - val);

    if (delta > deltamax) {
      delta = deltamax;
    }

    if (delta > 4)
      delta >>= 2;

    int16_t r = positive ? (prev + delta) : (prev - delta);

    prev = r;
    return r;
  }

private:
  int16_t prev;
  uint16_t deltamax;
};

#endif // FLTDEC_HPP
