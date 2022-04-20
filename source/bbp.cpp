#include "..\include\bbp.h"
#include "..\include\cppmath.h"
#include "..\include\numtypes.h"
#include <cfloat>

namespace pimath
{
    double sp(fast64 n, fast64 j)
    {
        double sum = 0.0;
        double denom = j;

        while (n >= 0)
        {
            sum = fp(sum + ((double)modpow(16, n, denom) / denom));
            denom += 8.0;
            --n;
        }

        double num = 0.0625;
        num /= denom;

        while (num > DBL_EPSILON)
        {
            sum += num;
            num *= 0.0078125;
        }

        return sum;
    }

    ufast8 pidec(fast64 n)
    {
        double sum =
          4.0 * sp(n, 1)
        - 2.0 * sp(n, 4)
        -       sp(n, 5)
        -       sp(n, 6);
        return (ufast8)(16.0 * fp(sum));
    }

    char pihex(fast64 n)
    {
        return tohex(pidec(n));
    }
}