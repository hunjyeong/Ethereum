#include <iostream>
#include <openssl/sha.h>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <cassert>
#include <random>
#include "EEA.hpp"
#include "finitefield.hpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;



uint512_t Gx_i = hexstr_to_uint512("0x40000000000000000000000000000000038aa1276c3f59b9a14064e200000000");
uint512_t Gy_i = hexstr_to_uint512("0x02");
uint512_t curve_order = hexstr_to_uint512("0x40000000000000000000000000000000038aa127696286c9842cafd400000001");

class Tweedledum {
public:
    Fp a = 0;
    Fp b = 5; 
    Fp x, y;
    bool infinity;

    Tweedledum() : x(0), y(0), infinity(false) {}

    Tweedledum(const Fp& x, const Fp& y) : x(x), y(y), infinity(false) {
        if(x==Fp(0)&&y==Fp(0)){
            infinity=true;
        }
    }

    Fp G_x = Gx_i;
    Fp G_y = Gy_i;
    
    static const Tweedledum G;

    uint512_t order = hexstr_to_uint512("0x40000000000000000000000000000000038aa127696286c9842cafd400000001");

    bool is_on_curve() const {
        if (infinity) return true;
        Fp L = y * y;
        Fp R = x * x * x + Fp(5);
        return L == R;
    }

    Tweedledum operator+(const Tweedledum& Q) const {
        if (infinity) return Q;
        if (Q.infinity) return *this;

        Fp s;
        if (*this == Q) {
            if (y == Fp(0)) return Tweedledum();
            s = (Fp(3) * x * x) / (Fp(2) * y);
        } else {
            if (x == Q.x) return Tweedledum();
            s = (y - Q.y) / (x - Q.x);
        }

        Fp x3 = s * s - x - Q.x;
        Fp y3 = s * (x - x3) - y;

        return Tweedledum(x3, y3);
    }

    Tweedledum operator-() const {
        return Tweedledum(x, -y);
    }

    Tweedledum operator-(const Tweedledum& Q) const {
        return *this + (-Q);
    }

    Tweedledum operator*(const uint512_t& k) const {
        Tweedledum R;
        Tweedledum S = *this;

        uint512_t m = k;
        while (m > 0) {
            if (m % 2 == 1) {
                R = R + S;
            }
            S = S + S;
            m /= 2;
        }

        return R;
    }

    Tweedledum operator*(const Fq& k) const {
        Tweedledum R;
        Tweedledum S = *this;

        uint512_t m = k.element; 
        while (m > 0) {
            if (m % 2 == 1) {
                R = R + S;
            }
            S = S + S;
            m /= 2;
        }

        return R;
    }
   

    bool operator==(const Tweedledum& Q) const {
        return (x == Q.x) && (y == Q.y);
    }

    bool operator!=(const Tweedledum& Q) const {
        return !(*this == Q);
    }

    static Tweedledum random_point() {
        Fp x, y;
        while (true) {
            x.random_element();
            Fp tmp = x * x * x + Fp(5);
            if (tmp.legendre() == 1) {
                y = tmp.sqrt();
                break;
            }
        }
        return Tweedledum(x, y);
    }

    static Tweedledum get_generator() {
        return G;
    }

    static Tweedledum get_random_point() {
        Tweedledum G = get_generator();

        std::random_device rd;
        uint512_t random_value = rd();
        for (int i = 0; i < 9; ++i) {
            random_value <<= 64;
            random_value |= rd();
        }
        random_value %= curve_order; 

        random_value %= (curve_order - 1);
        random_value += 1;

        return G * random_value;
    }

    



    friend std::ostream& operator<<(std::ostream& out, const Tweedledum& P) {
        if (P.infinity) {
            out << "Infinity";
        } else {
            out << "(" << P.x << ", " << P.y << ")";
        }
        return out;
    }
    
};

Tweedledum operator*(const Fq& k, const Tweedledum& P) {
    return P * k;
}

Tweedledum operator*(int k, const Tweedledum& P) {
    return P * uint512_t(k);
}

Tweedledum operator*(const Tweedledum& P, int k) {
    return P * uint512_t(k);
}

const Tweedledum Tweedledum::G = Tweedledum(Fp(hexstr_to_uint512("0x40000000000000000000000000000000038aa1276c3f59b9a14064e200000000")), Fp(hexstr_to_uint512("0x02")));

