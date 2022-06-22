#ifndef MATHDEFS_H
#define MATHDEFS_H

#include <math.h>

#define PI 3.14159265

#ifndef APPROX
#define APPROX(POS, V, L) ((POS) * (V) / (L))
#endif

#define ROUNDMASK(BITS) ((1 << BITS) - 1)
#define ROUNDANG(V) (V & ROUNDMASK(12))

#define DEG0 0
#define DEG360 4096

#define DEG180 (DEG360 >> 1)

#define DEG90 (DEG180 >> 1)
#define DEG270 (DEG180 + DEG90)

#define DEG45 (DEG90 >> 1)
#define DEG315 (DEG270 + DEG45)

#define DEG30 (DEG90 / 3)
#define DEG330 (DEG360 - DEG30)

// min max values
#define I16MAX (32767)
#define I16MIN (-32767)

// sin 0`
#define SIN1V(V) (sintbl[V])

// sin 90`
#define SIN2V(V) (sintbl[1023 - V])

// sin 180`
#define SIN3V(V) (0 - sintbl[V])

// sin 270`
#define SIN4V(V) (0 - sintbl[1023 - V])

// sin 0` - 1
#define SIN1D(V) (I16MIN + sintbl[V])

// sin 90` - 1
#define SIN2D(V) (I16MIN + sintbl[1023 - V])

// sin 270` + 1
#define SIN3U(V) (I16MAX - sintbl[V])

// sin 270` + 1
#define SIN4U(V) (I16MAX - sintbl[1023 - V])

#define SAW1(V) APPROX(V, I16MAX, 1024)
#define SAW2(V) APPROX((1023 - V), I16MAX, 1024)
#define SAW3(V) 0 - APPROX(tblangle, I16MAX, 1024)
#define SAW4(V) 0 - APPROX(1023 - tblangle, I16MAX, 1024)

#define MEA1 I16MAX
#define MEA2 I16MAX
#define MEA3 I16MIN
#define MEA4 I16MIN

#endif // MATHDEFS_H
