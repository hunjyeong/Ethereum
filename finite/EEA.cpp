#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

uint512_t extended_gcd(uint512_t a, uint512_t b, int512_t& x, int512_t& y)  
{
    if (b == 0) {
        x = 1;
        y = 0;
        
        return a;
    }
    
    int512_t x1, y1;
    uint512_t gcd = extended_gcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return gcd;

}

uint512_t mod_inverse(uint512_t a, uint512_t m)
{
        
    int512_t x, y;
    extended_gcd(a, m, x, y);
    int512_t mod_result = x % m;
    return (mod_result >= 0) ? uint512_t(mod_result) : uint512_t(mod_result + m);

}
