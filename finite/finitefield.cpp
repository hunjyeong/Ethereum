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
#include "EEA.hpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

uint512_t p("28948022309329048855892746252171976963322203655955319056773317069363642105857");
uint512_t q("28948022309329048855892746252171976963322203655954433126947083963168578338817");


class Fp {

    public:
        uint512_t element;
        uint512_t modul=p;
        
        Fp() : element(0) {}
        Fp(uint512_t a) : element(a%p), modul(p) {}
        Fp(int a) : element(a%p), modul(p) {}
        
        Fp& operator=(const uint512_t& a){

            element = a%p;

            return *this;

        }
        

        friend std::ostream& operator << (std::ostream& out, const Fp& Fp){

            out<<Fp.element;

            return out;

        }

        operator uint512_t() const {
            return element;
        }

        operator int512_t() const {
            return static_cast<int512_t>(element);
        }


        //+ 정의
        Fp operator+(const Fp& a) const {

            assert(modul == a.modul);
            uint512_t res = (element + a.element) % modul;
            return Fp(res); 

        }

        //- 정의
        Fp operator-(const Fp& a) const {

            assert(modul == a.modul);
            uint512_t res = (modul + element - a.element) % modul;
            return Fp(res);

        }

        //* 정의
        Fp operator*(const Fp& a) const {

            assert(modul == a.modul);
            uint512_t res = (element * a.element) % modul;

            return Fp(res);

        }

        // /정의
        Fp operator/(const Fp& a) const {

            assert(modul == a.modul);
            uint512_t inv = mod_inverse(a.element, modul);
            uint512_t res = (element * inv) % modul;

            return Fp(res);

        }

        //음수부호 붙이기 정의
        Fp operator-() const {
            
            uint512_t res = (modul - element) % modul;
            return Fp(res);

        }

        //exp 정의
        Fp pow(const int512_t& exp) const {

            Fp base = element;
            Fp res = 1;
            int512_t e = exp;

            if (e < 0) {
                uint512_t inv = mod_inverse(element, modul);
                base = Fp(inv);
                e = -e;               
            } else{
                e = e;
            }

                
            while(e > 0){
                
                if(e%2 == 1){
                    res = res * base;
  
                }
                
                base = base * base;
                e = e/2;
                
            }
            
            return res;

        }

        template<typename T>
        Fp pow(T exp) const {
            return pow(int512_t(exp));
        }


        Fp pow(const Fp& exp) const {

            return pow(int512_t(exp.element));

        }

        // Fp sqrt() const {

        //     assert(legendre_symbol(element, modul)==1);

        //     return pow(p*(Fp(1)/Fp(2)));


        // }


        //비교연산자들
        bool operator==(const Fp& a) const {
            assert(modul == a.modul);
            return element == a.element;
        }

    
        bool operator!=(const Fp& a) const {
            assert(modul == a.modul);
            return element != a.element;
        }


        static Fp random_element() {
            std::random_device rd;
            uint512_t random_value = rd();
            for (int i = 0; i < 9; ++i) {
                random_value <<= 64;
                random_value |= rd();
            }
            random_value %= p; 

            return Fp(random_value);
        }

        
        // static int legendre_symbol(const uint512_t& a, const uint512_t& p) {
        //     Fp a_fp(a);
        //     Fp ls = a_fp.pow((p - 1) / 2);
        //     uint512_t ls_uint = static_cast<uint512_t>(ls);
        //     return (ls_uint == p - 1) ? -1 : 1;
        // }

        int legendre() const {
            Fp result = this->pow((p - 1) / 2);
            if (result.element == 1) {
                return 1;
            } else if (result.element == 0) {
                return 0;
            } else {
                return -1;
            }
        }

        Fp sqrt() const {
            if (this->legendre() != 1) {
                throw std::runtime_error("No square root exists");
            }

            if (p % 4 == 3) {
                return this->pow((p + 1) / 4);
            }

            uint512_t q = p - 1;
            int s = 0;
            while (q % 2 == 0) {
                q /= 2;
                s++;
            }

            Fp z(2);
            while (z.legendre() != -1) {
                z.element += 1;
            }

            Fp c = z.pow(q);
            Fp r = this->pow((q + 1) / 2);
            Fp t = this->pow(q);
            int m = s;

            while (t.element != 1) {
                int i = 0;
                Fp tmp = t;
                while (tmp.element != 1 && i < m) {
                    tmp = tmp * tmp;
                    i++;
                }

                Fp b = c;
                for (int j = 0; j < m - i - 1; j++) {
                    b = b * b;
                }

                r = r * b;
                c = b * b;
                t = t * c;
                m = i;
            }

            return r;
        }
        

};
/*int main(){

    cout<<"cout test"<<endl;
    Fp a = 1;
    cout<<a<<endl;
    cout<<"======================================"<<endl;

    cout<<"+ test"<<endl;
    Fp b = p+3;
    Fp c = a+b;
    cout<<c+3<<endl;
    cout<<"======================================"<<endl;

    cout<<"- test"<<endl;
    Fp d = 4;
    Fp e = d-1;
    cout<<e<<endl;
    cout<<"======================================"<<endl;

    cout<<"-() test"<<endl;
    Fp f = 6;
    Fp g = -f;
    Fp h = f+g;
    cout<<g<<endl;
    cout<<h<<endl;
    cout<<"======================================"<<endl;

    cout<<"* test"<<endl;
    Fp i = 3;
    Fp j;
    j = i;
    Fp k =i*j;
    cout<<k<<endl;
    cout<<"======================================"<<endl;

    cout<<"pow test"<<endl;
    Fp l = 2;
    Fp m = 255;
    uint512_t exp = 255;
    Fp power = l.pow(exp);
    Fp power2 = l.pow(m);
    cout<<power<<endl;
    cout<<power2<<endl;
    cout<<"======================================"<<endl;

    cout<<"/ test"<<endl;
    Fp n = 1;
    Fp o = 3;
    Fp pp = n/o;
    Fp qq = pp*9;
    cout<<pp<<endl;
    cout<<qq<<endl;
    cout<<"======================================"<<endl;

    cout<<"pow test : exp<0 "<<endl;
    Fp a1 = 123;
    Fp b1 = a1.pow(-4);
    cout<<b1*123*123*123*123<<endl;
    cout<<"======================================"<<endl;

    cout<<"==, != test"<<endl;

    cout<<(l==m)<<" "<<(power==power2)<<" "<<(l!=m)<<endl;
    cout<<"======================================"<<endl;

    cout<<"rand test"<<endl;

    Fp k1 = Fp::random_element();
    cout<<k1<<endl;
    cout<<"======================================"<<endl;

    Fp er =3;
    Fp kr = er.pow(p+1);
    cout<<kr*er<<endl;

    cout<<"======================================"<<endl;

    Fp ert = 16;
    uint512_t exp2 = p-2;
    Fp errr = ert.pow(exp2);
    cout<<errr<<endl;
    cout<<errr*ert<<endl;
    

    cout<<"======================================"<<endl;

    cout<<"르장드르 기호"<<endl;
    Fp CA = 36;
    Fp CB = 7;
    cout<<CA.legendre()<<endl;
    cout<<CB.legendre()<<endl;
    cout<<p%4<<endl;

    cout<<"======================================"<<endl;

    cout<<"sqrt"<<endl;
    cout<<CA.sqrt()*CA.sqrt()<<endl;

}*/