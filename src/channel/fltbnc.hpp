#ifndef FLTBNC_HPP
#define FLTBNC_HPP

#include <stdint.h>

#define SAFEADD(A, B, MAXVAL)                                                  \
  {                                                                            \
    int cap = MAXVAL - A;                                                      \
    int dy = ((B) > cap) ? cap : (B);                                          \
    A += dy;                                                                   \
  }

#define SAFESUB(A, B, MINVAL)                                                  \
  {                                                                            \
    auto cap = A - MINVAL;                                                     \
    auto dy = ((B) > cap) ? cap : (B);                                         \
    A -= dy;                                                                   \
  }

class FLTBnc {

public:
  FLTBnc() : prev(0) {}

  int16_t ins(int16_t val) {

    bool dirpos = val >= prev;
    int16_t dy = (dirpos ? (val - prev) : (prev - val)) * 2;

    if (dy != 0) {

      if (dirpos) {

	int cap = INT16_MAX - acc;
	int add = (dy > cap) ? cap : dy;
	acc += add;

	// SAFEADD(acc, dy, INT16_MAX);

      } else {

	int cap = acc - (0 - INT16_MAX);
	int sub = (dy > cap) ? cap : dy;
	acc -= sub;
      }
    }

    int16_t r = val + acc;

    if (acc) {
      acc -= (acc / 2);
    } else {
      acc += (acc / 2);
    }

    prev = val;

    return r;
  }

private:
  int16_t prev = 0;
  int16_t acc = 0;
};

#endif // FLTBNC_HPP
