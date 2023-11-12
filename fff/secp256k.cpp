#define secp256k

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



uint512_t G_x_ = hexstr_to_uint512("0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798");
uint512_t G_y_ = hexstr_to_uint512("0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8");
uint512_t curve_order_ = hexstr_to_uint512("0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");

class secp256k1 {
public:
    Fp a = 0;
    Fp b = 7; 
    Fp x, y;
    bool infinity;

    secp256k1() : x(0), y(0), infinity(true) {}

    secp256k1(const Fp& x, const Fp& y) : x(x), y(y), infinity(false) {
    }

    Fp G_x = G_x_;
    Fp G_y = G_y_;
    
    static const secp256k1 G;

    uint512_t order = hexstr_to_uint512("0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");

    bool is_on_curve() const {
        if (infinity) return true;
        Fp L = y * y;
        Fp R = x * x * x + Fp(7);
        return L == R;
    }

    secp256k1 operator+(const secp256k1& Q) const {
        if (infinity) return Q;
        if (Q.infinity) return *this;

        Fp s;
        if (*this == Q) {
            if (y == Fp(0)) return secp256k1();
            s = (Fp(3) * x * x) / (Fp(2) * y);
        } else {
            if (x == Q.x) return secp256k1();
            s = (y - Q.y) / (x - Q.x);
        }

        Fp x3 = s * s - x - Q.x;
        Fp y3 = s * (x - x3) - y;

        return secp256k1(x3, y3);
    }

    secp256k1 operator-() const {
        return secp256k1(x, -y);
    }

    secp256k1 operator-(const secp256k1& Q) const {
        return *this + (-Q);
    }

    secp256k1 operator*(const uint512_t& k) const {
        secp256k1 R;
        secp256k1 S = *this;

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

    secp256k1 operator*(const int &k) const {
        secp256k1 R;
        secp256k1 S = *this;

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

    secp256k1 operator*(const Fq &k) const {
        secp256k1 R;
        secp256k1 S = *this;

        R = S*k.element;

        return R;
    }
   

    bool operator==(const secp256k1& Q) const {
        return (x == Q.x) && (y == Q.y);
    }

    bool operator!=(const secp256k1& Q) const {
        return !(*this == Q);
    }

    static secp256k1 random_point() {
        Fp x, y;
        while (true) {
            x.random_element();
            Fp tmp = x * x * x + Fp(7);
            if (tmp.legendre() == 1) {
                y = tmp.sqrt();
                break;
            }
        }
        return secp256k1(x, y);
    }

    static secp256k1 get_generator() {
        return G;
    }

    static secp256k1 get_random_point() {
        secp256k1 G = get_generator();

        std::random_device rd;
        uint512_t random_value = rd();
        for (int i = 0; i < 9; ++i) {
            random_value <<= 64;
            random_value |= rd();
        }
        random_value %= curve_order_; 

        random_value %= (curve_order_ - 1);
        random_value += 1;

        return G * random_value;
    }

    



    friend std::ostream& operator<<(std::ostream& out, const secp256k1& P) {
        if (P.infinity) {
            out << "Infinity";
        } else {
            out << "(" << P.x << ", " << P.y << ")";
        }
        return out;
    }
    
};

const secp256k1 secp256k1::G = secp256k1(Fp(hexstr_to_uint512("0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798")), Fp(hexstr_to_uint512("0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8")));
