#include "..\include\numtypes.h"
#include "..\include\cppmath.h"

namespace pimath
{
    char tohex(ufast8 a)
    {
        if (a > 9) return a + 55;
        return a + 48;
    }

    fast64 modpow(fast64 b, fast64 e, fast64 m)
    {
        fast64 res = 1;
        while (e)
        {
            if (e & 1)
                res = (res * b) % m;
            e >>= 1;
            b = b * b % m;
        }
        return res;
    }

    double fp(double a)
    {
        return a - ((fast64)(a + 10000) - 10000);
    }
}