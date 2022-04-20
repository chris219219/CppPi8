#ifndef CPPMATH_H
#define CPPMATH_H

#include "numtypes.h"

namespace pimath
{
    char tohex(ufast8 a);
    fast64 modpow(fast64 b, fast64 e, fast64 m);
    double fp(double a);
}

#endif