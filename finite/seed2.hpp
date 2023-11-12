#define secp256k

#ifndef SEED2_H__
#define SEED2_H__
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
#include "seed2.cpp"
#include "EEA.hpp"
#include "finitefield.hpp"
#include "secp256k.hpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

inline void put(const char *msg, const vector<unsigned char> x);
string uint512_to_string(uint512_t num);

string str_hex(const vector<unsigned char>& data);
string bytes_to_hex_string(const std::vector<uint8_t>& bytes);
string uint512_to_hex(uint512_t num);

string hex_str_of_usarr(unsigned char* in, size_t len);

string sha3_256(const std::string& input);

string SHA256(const string strIn);


uint128_t CSPRNG();

string tobinary(uint128_t n);

string checksum(string n);



string Mnemonic(string *indice, string *wordlist);

void printuchar(const unsigned char* a);


struct seed;

struct key;

#endif


// int main() {
//     // seed new_seed;
//     // key master_priv;
    
//     // new_seed.Mnemonic_gen();
    
//     seed new_seed;
//     key master_priv;
    
//     new_seed.Mnemonic_gen();
//     new_seed.seed_gen(); //seed ->512

//     master_priv.cut_key(new_seed.out);
//     cout<<master_priv.key_val<<endl;
//     string tmppp = "c17075bf07a2185bee53696474075380fe7a5c0a28f2b73b5cc94cec5a271a76";
//     string tmppp2 = "0x";
//     string tmp = tmppp2 + tmppp;

//     uint512_t insp(tmp);
//     cout<<insp<<endl;
//     // cout<<master_priv.key_val.size()<<endl;
//     // cout<<master_priv.chain_code.size()<<endl;
    
//     // string pub = master_priv.public_gen();
//     // cout<<pub<<endl;

//     // string hash = sha3_256(pub);
//     // cout<<hash<<endl;
    

//     // string prefix;
//     // string pubkey;

//     // uint512_t val = hexstr_to_uint512(master_priv.key_val);
//     // secp256k1 gen = secp256k1::get_generator();
//     // secp256k1 pub = gen * val;

//     // if(pub.y.element% 2 == 1){
//     //         prefix ="0x03";
//     // }else{
//     //         prefix ="0x02";
//     // }
            
//     // pubkey = prefix + uint512_to_string(pub.x.element);
//     // cout<<pubkey<<endl;

//     // for(int i=0;i<100;i++){
//     //     cout<<tobinary(CSPRNG())<<endl;
//     // }
//     // boost::multiprecision::uint512_t value = 12345678901234567890;
//     // std::ostringstream oss;
//     // oss << value;
//     // std::string str_value = oss.str();

//     // std::cout << "Value as string: " << str_value << std::endl;

//     // uint512_t tmp("9920964227396436201098134567662456251840087529114724759170449860433124404306");
//     // cout<<tmp.str()<<endl;

//     // for(int i=0;i<100;i++){

//     //     seed hello;
//     //     cout<<hello.ent<<endl;
        

//     // }


// }