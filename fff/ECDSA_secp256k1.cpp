#define secp256k

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
#include "seed2.hpp"
#include "secp256k.hpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

// struct Sig {
//     Fq r;
//     Fq s;
// };

// Sig Sig_Gen(const key& privateKey, const std::string& message) {
    
//     string hashStr = sha3_256(message);
//     uint512_t e = hexstr_to_uint512(hashStr);
//     Fq k = Fq::random_element();
//     secp256k1 G = secp256k1::get_generator();
//     secp256k1 P_1 = G*k;
//     while(P_1.x==0){
//         k = Fq::random_element();
//         P_1 = G*k;
//     }
//     Fq r(P_1.x.element);
//     Fq d(privateKey.key_val);

//     Fq s = (Fq(e) + d*r)/k;
//     cout<<"Sig_Gen"<<endl;
//     cout<<"==============================================================================================================="<<endl;
//     cout<<"message : "<<message<<endl;
//     cout<<"r : "<<r<<endl;
//     cout<<"s : "<<s<<endl;
//     cout<<"==============================================================================================================="<<endl;
//     cout<<endl;

//     return Sig {r,s};

// }

// bool Sig_Verify(const secp256k1& pubkey, const string& message, const Sig& sig) {

//     bool flag = 0;
//     //assert((pubkey!=secp256k1(0,0))&&(pubkey*q==secp256k1(0,0)));
//     assert((sig.r.element<q)&&(sig.s.element<q));

//     Fq w = Fq(1)/sig.s;

//     string hashStr = sha3_256(message);
//     uint512_t e = hexstr_to_uint512(hashStr);

//     Fq u1 = Fq(e)*w;
//     Fq u2 = sig.r*w;

//     secp256k1 V = (secp256k1::G)*u1 + pubkey*u2;
//     assert(V!=secp256k1(0,0));

//     Fq v = Fq(V.x.element);
//     flag = (sig.r == v);
//     cout<<"Sig_Verify"<<endl;
//     cout<<"==============================================================================================================="<<endl;
//     cout<<"message : "<<message<<endl;
//     cout<<"r : "<<sig.r<<endl;
//     cout<<"s : "<<sig.s<<endl;
//     cout<<"---------------------------------------------------------------------------------------------------------------"<<endl;
//     if(flag==true){
//         cout<<"The signature is valid"<<endl;
//     }else{
//         cout<<"The signature is not vaild!!!"<<endl;
//     }
//     cout<<"==============================================================================================================="<<endl;
//     cout<<endl;

//     return flag;
// }

