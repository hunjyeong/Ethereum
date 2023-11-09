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

uint512_t hexstr_to_uint512(const std::string& hex_str) {
    uint512_t result;
    std::stringstream ss;
    ss << std::hex << hex_str;
    ss >> result;
    return result;
}

uint512_t Gx_i = hexstr_to_uint512("0x40000000000000000000000000000000038aa1276c3f59b9a14064e200000000");
uint512_t Gy_i = hexstr_to_uint512("0x02");
uint512_t curve_order = hexstr_to_uint512("0x40000000000000000000000000000000038aa127696286c9842cafd400000001");

class Tweedledum {
public:
    Fp a = 0;
    Fp b = 5; 
    Fp x, y;
    bool infinity;

    Tweedledum() : x(0), y(0), infinity(true) {}

    Tweedledum(const Fp& x, const Fp& y) : x(x), y(y), infinity(false) {
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

const Tweedledum Tweedledum::G = Tweedledum(Fp(hexstr_to_uint512("0x40000000000000000000000000000000038aa1276c3f59b9a14064e200000000")), Fp(hexstr_to_uint512("0x02")));

// int main(){

//     Fp x1(13);
//     Fp y1(uint512_t("842475444227385451567746113014170960838897203331904475335665653869162140916"));
//     Fp x2(13);
//     Fp y2(uint512_t("842475444227385451567746113014170960838897203331904475335665653869162140916"));

//     Tweedledum P1(x1, y1);
//     Tweedledum P2(x2, y2);

//     cout<<"is_on_curve test"<<endl;
//     cout<<P1.is_on_curve()<<endl;
//     cout<<P1.is_on_curve()<<endl;
//     cout<<"================================================="<<endl;

//     Tweedledum P3 = P1 + P1;
//     Tweedledum P4 = P1 - P2;

//     Tweedledum P5 = P1*2;
//     Tweedledum P6 = P1 + (-P2);

//     cout<< (P3 == P5) <<endl;
//     cout<< (P4 == P6) <<endl;

//     cout<< (P3 == P4) <<endl;

//     cout<<"================================================="<<endl;

//     cout<<"rand test"<<endl;

//     Tweedledum random_point = Tweedledum::get_random_point();
//     std::cout << "랜덤한 점: " << random_point << std::endl;
//     cout<<random_point.is_on_curve()<<endl;

//     cout<<"================================================="<<endl;

//     cout<<"get generator test"<<endl;

    
//     Tweedledum G = Tweedledum::get_generator();
//     cout << "Tweedledum curve generator: " << G << endl;
//     cout<<G.is_on_curve()<<endl;

//     cout<<"================================================="<<endl;

//     Tweedledum A = Tweedledum::get_random_point();
//     Tweedledum B = A*4;
//     Tweedledum C = A+A+A+A;

//     Tweedledum D = Tweedledum::get_random_point();
//     Tweedledum E = D*5;
//     Tweedledum F = D+D+D+D+D;

//     cout<<"A is on?: "<<A.is_on_curve()<<endl;
//     cout<<"B is on?: "<<B.is_on_curve()<<endl;
//     cout<<"C is on?: "<<C.is_on_curve()<<endl;
//     cout<<"D is on?: "<<D.is_on_curve()<<endl;
//     cout<<"E is on?: "<<E.is_on_curve()<<endl;
//     cout<<"F is on?: "<<F.is_on_curve()<<endl;
//     cout<<" "<<endl;


//     cout<<"A*4 = A+A+A+A ?:"<<(B==C)<<endl;
//     cout<<"D*5 = D+D+D+D+D ?:"<<(E==F)<<endl;
//     cout<<" "<<endl;

//     cout<<"A = "<<A<<endl;
//     cout<<"B = "<<B<<endl;
//     cout<<"C = "<<C<<endl;
//     cout<<"D = "<<D<<endl;
//     cout<<"E = "<<E<<endl;
//     cout<<"F = "<<F<<endl;
    

// }