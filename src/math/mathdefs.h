#ifndef MATHDEFS_H
#define MATHDEFS_H

#include <math.h>

#define PI 3.14159265

#ifndef APPROX
#define APPROX(POS, V, L) ((POS) * (V) / (L))
#endif

#define ROUNDMASK(BITS) ((1 << BITS) - 1)
#define ROUNDANG(V) (V & ROUNDMASK(12))

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

#define SAW1(V) APPROX(V, I16MAX, 1024)
#define SAW2(V) APPROX((1023 - V), I16MAX, 1024)
#define SAW3(V) 0 - APPROX(tblangle, I16MAX, 1024)
#define SAW4(V) 0 - APPROX(1023 - tblangle, I16MAX, 1024)

#endif // MATHDEFS_H
