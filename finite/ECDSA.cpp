#include <iostream>
#include <openssl/sha.h>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/algorithm/hex.hpp>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <locale>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <boost/lexical_cast.hpp>
#include "EEA.hpp"
#include "finitefield.hpp"
#include "ellipticcurve.hpp"
#include "seed1.hpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

struct sig {
    public:
        Fp r;
        Fp s;
    
    
    void sign_gen(string mes, key priv_key) {
        string message = sha3_256(mes); //message hash
        uint512_t hash_val = hexstr_to_uint512(message); //
        Fp hash = hash_val;
        Fp prv = hexstr_to_uint512(priv_key.key_val);

        Fp k = Fp::random_element();
        Tweedledum gen = Tweedledum::get_generator();
        Tweedledum tmp = gen.Tweedledum::operator*(k.element);

        this->r = tmp.x;
        Fp inv_k = mod_inverse(k.element, k.modul);
        cout<<inv_k * k<<endl;
        this->s = (hash + (this->r * prv)) * inv_k;
        cout<<this->s * k<<endl;
    
    }
    
};

bool sign_verification(string mes, Tweedledum pub_key, sig psign){
    string message = sha3_256(mes);
    uint512_t hash_val = hexstr_to_uint512(message);
    Fp hash = hash_val;

    Fp inv_s = mod_inverse(psign.s, psign.s.modul);

    Tweedledum gen = Tweedledum::get_generator();
    Tweedledum vpoint = gen.Tweedledum::operator*(hash * inv_s) + pub_key.Tweedledum::operator*(psign.r * inv_s);


    return (psign.r == vpoint.x);
}

int main(){
    // seed new_seed;
    // key master_priv;
    
    // new_seed.Mnemonic_gen();

    // new_seed.seed_gen(); //seed 

    // master_priv.cut_key(new_seed.out);
    
    //check
    //message
    string mes = "hello";
    Fp priv = 4;

    string message = sha3_256(mes);
    //cout<<message<<endl; "3338be694f50c5f338814986cdf0686453a888b84f424d792af4b9202398f392" hexa string
    uint512_t hash_val = hexstr_to_uint512(message);
    //cout<<hash_val<<endl; 
    //"23168212884327364813911369722891485169665316098928853543147410660570078704530" (10)
    Fp hash = hash_val;
    //equal
    
    //Fp k = Fp::random_element();
    Fp k = 3; //instead of random val
    //cout<<(k.modul == hash.modul)<<endl; ==1
    Tweedledum gen = Tweedledum::get_generator();
    Tweedledum tmp = gen * k.element;
    // cout<<tmp<<endl;

    //(26586918612200025172663574889652563637597247063518212393170691946747032331143, 
    //3576904064972091952554328254785285344757641953309388400080693193037794205553)

    // cout<<(tmp == (gen + gen + gen))<<endl;
    // uint512_t s = k.modul+3;
    // Fp u = s;
    // cout<<gen * u<<endl;

    Fp r = tmp.G_x;
    Fp inv_k = mod_inverse(k.element, k.modul);
    //cout<<inv_k * k<<endl; ==1
    Fp s = (hash + (r * priv)) * inv_k;
    cout<<"r = "<<endl;
    cout<<r<<endl;
    cout<<"--------------"<<endl;
    cout<<"s = "<<endl;
    cout<<s<<endl;

    cout<<"verification"<<endl;

    Fp inv_s = mod_inverse(s.element, s.modul);
    //cout<< s * inv_s <<endl; == 1

    Tweedledum pub = gen * priv;
    //cout<<pub<<endl;
    //cout<<((pub - gen - gen - gen) == gen)<<endl; == 1
    //cout<<(hash * inv_s)+(priv * r * inv_s)<<endl; == 3
    //cout<< ((gen * 3)==(gen*((hash * inv_s)+(priv * r * inv_s))))<<endl; ==1
    
    // cout<<(gen*((hash * inv_s)+(priv * r * inv_s)))<<endl;
    // cout<<((gen * (hash * inv_s)) + (gen * priv * (r * inv_s)))<<endl; 둘이 다름



    Fp u1 = hash * inv_s;
    Tweedledum tmp1 = gen * u1;
    //cout<<u1<<endl;

    Fp u2 = r * inv_s;
    Tweedledum tmp2 = pub * u2;
    //cout<<u2<<endl;
    //cout<<u1+u2<<endl; == 3
    //cout<<(gen * k == gen * (u1 + u2))<<endl; ==1
    //cout<<((gen * u1) == (gen * (hash * inv_s)))<<endl; == 1
    //cout<<((gen * u2) == (gen * (priv * r * inv_s)))<<endl; ==1
    //cout<<((gen * u2) == (gen * priv * ( r * inv_s)))<<endl; ==1
    //cout<<((gen * priv * ( r * inv_s)) == (pub * (r * inv_s)))<<endl; ==1
    //cout<<((gen * u2) == (pub * (r * inv_s)))<<endl; ==1
    //cout<<(tmp2 == (gen * priv * ( r * inv_s)))<<endl; ==1
    //cout<<((tmp1 + tmp2) == ((gen * (hash * inv_s))+(gen * priv * ( r * inv_s))))<<endl; ==1
    //cout<<((tmp1 + tmp2) == (gen * ((hash * inv_s) + (priv * r * inv_s))))<<endl; == 0 ?????
    cout<<(hash * inv_s)+(priv * r * inv_s)<<endl;
    cout<<gen * ((hash * inv_s)+(priv * r * inv_s))<<endl;
    cout<<gen * 3<<endl;
    cout<<(gen * (hash * inv_s)) + (gen * (priv * r * inv_s))<<endl;
    //Tweedledum verify_point = tmp1 + tmp2;
    //cout<<(verify_point == (gen * k))<<endl; == 0????

    Tweedledum tmp3 = gen * (hash + r * priv)* inv_s;
    // cout<<verify_point<<endl;
    // cout<<tmp3<<endl;
    // cout << (verify_point == tmp3) << endl; ==0 ??????




    // Tweedledum vpoint = gen *(hash * inv_s) + pub_key.Tweedledum::operator*(psign.r * inv_s);
