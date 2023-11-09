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

struct Sig {
    Fq r;
    Fq s;
    Tweedledum v;
};

Sig Sig_Gen(const key& privateKey, const string& message) {
    
    string hashStr = sha3_256(message);
    uint512_t e = hexstr_to_uint512(hashStr);
    Fq k = Fq::random_element();
    
    cout<<"random k is below"<<endl;
    cout<<k<<endl;
    cout<<"---------------"<<endl;

    Tweedledum G = Tweedledum::get_generator();
    Tweedledum P_1 = G*k;
    while(P_1.x==0){
        k = Fq::random_element();
        P_1 = G*k;
    }
    Fq r(P_1.x.element);
    Fq d(privateKey.key_val);

    Fq s = (Fq(e) + d*r)/k;

    //v : public_key information
    Tweedledum v = G * d;

    cout<<"Sig_Gen"<<endl;
    cout<<"==============================================================================================================="<<endl;
    cout<<"message : "<<message<<endl;
    cout<<"r : "<<r<<endl;
    cout<<"s : "<<s<<endl;
    cout<<"==============================================================================================================="<<endl;
    cout<<endl;

    return Sig {r,s, v};

}


bool Sig_Verify(const Tweedledum& pubkey, const string& message, const Sig& sig) {

    bool flag = 0;
    assert((pubkey!=Tweedledum(0,0))&&(pubkey*q==Tweedledum(0,0)));
    assert((sig.r.element<q)&&(sig.s.element<q));

    Fq w = Fq(1)/sig.s;

    string hashStr = sha3_256(message);
    uint512_t e = hexstr_to_uint512(hashStr);

    Fq u1 = Fq(e)*w;
    Fq u2 = sig.r*w;

    Tweedledum V = (Tweedledum::G)*u1 + pubkey*u2;
    assert(V!=Tweedledum(0,0));

    Fq v = Fq(V.x.element);
    flag = (sig.r == v);
    cout<<"Sig_Verify"<<endl;
    cout<<"==============================================================================================================="<<endl;
    cout<<"message : "<<message<<endl;
    cout<<"r : "<<sig.r<<endl;
    cout<<"s : "<<sig.s<<endl;
    cout<<"---------------------------------------------------------------------------------------------------------------"<<endl;
    if(flag==true){
        cout<<"The signature is valid"<<endl;
    }else{
        cout<<"The signature is not vaild!!!"<<endl;
    }
    cout<<"==============================================================================================================="<<endl;
    cout<<endl;

    return flag;
}


// int main() {
    
//     key privateKey;
//     privateKey.key_val = "0x123456789abcdef";  
//     std::string message = "Hello";
    
//     Sig sig = Sig_Gen(privateKey,message);

//     Tweedledum G = Tweedledum::G;
//     Fq d(privateKey.key_val);
//     Tweedledum pubkey = G*d;

//     Sig_Verify(pubkey,"Hello",sig);

//     return 0;
// }
