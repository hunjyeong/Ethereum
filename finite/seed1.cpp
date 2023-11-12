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
#include "transaction.hpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

inline void put(const char *msg, const vector<unsigned char> x)
{
	printf("%s=", msg);
	for (size_t i = 0; i < x.size(); i++) {
		printf("%s", &x[i]);
	}
	printf("\n");
}

string uint512_to_string(uint512_t num) {
    ostringstream oss;
    oss << num;
    string str_value = oss.str();

    return str_value;
}

string str_hex(const vector<unsigned char>& data) {
    ostringstream oss;

    for(const auto& byte : data) {
        oss<<hex<<static_cast<int>(byte);
    }
    string hex_str = oss.str();
    return hex_str;
} //pbkdf2
string bytes_to_hex_string(const std::vector<uint8_t>& bytes)
{
    std::ostringstream stream;
    for (uint8_t b : bytes)
    {
        stream << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(b);
    }
    return stream.str();
}

string uint512_to_hex(uint512_t num){
    ostringstream ss;
    ss << hex << num;
    string result = ss.str();

    return result;
}

string hex_str_of_usarr(unsigned char* in, size_t len){

    ostringstream ost;
    ost<<hex<<setfill('0');
    for(size_t i=0;i<len;i++){
        ost<<std::setw(2)<<static_cast<int>(in[i]);
    }
    return ost.str();
}

string sha3_256(const std::string& input)
{
    uint32_t digest_length = SHA512_DIGEST_LENGTH;
    const EVP_MD* algorithm = EVP_sha3_256();
    uint8_t* digest = static_cast<uint8_t*>(OPENSSL_malloc(digest_length));
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    EVP_DigestInit_ex(context, algorithm, nullptr);
    EVP_DigestUpdate(context, input.c_str(), input.size());
    EVP_DigestFinal_ex(context, digest, &digest_length);
    EVP_MD_CTX_destroy(context);
    std::string output = bytes_to_hex_string(std::vector<uint8_t>(digest, digest + digest_length));
    OPENSSL_free(digest);
    return output;
}

string SHA256(const string strIn){

        BYTE hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, strIn.c_str(), strIn.size());
        SHA256_Final(hash, &sha256);
        stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
        return ss.str();
}


uint128_t CSPRNG(){

    random_device rnd;
    uint64_t t1 = rnd();
    uint64_t t2 = rnd();
    uint64_t t3 = rnd();
    uint64_t t4 = rnd();

    string buf = to_string(t1)+to_string(t2)+to_string(t3)+to_string(t4);;

    return uint128_t(buf);
    //16bytes = 128bits
}

string tobinary(uint128_t n) {
    uint32_t binary[128];
    std::ostringstream os;
    
    for (uint32_t i=0;i<128;i++){
        if (n % 2 != 0) {
            binary[i] = 1;
        }
        else{
            binary[i] = 0;
        }
        n = n/2;
    }

    for (int j: binary) {
        os << j;
    }
 
    string str(os.str());
    return str;

}

string checksum(string n) {
    uint32_t binary[256];
    uint256_t val;
    stringstream ss;
    ss << n;
    ss >> val;
    std::ostringstream os;
    
    for (uint32_t i=0;i<256;i++){
        if (val % 2 != 0) {
            binary[i] = 1;
        }
        else{
            binary[i] = 0;
        }
        val = val/2;
    }

    for (int j: binary) {
        os << j;
    }
 
    string str(os.str());
    return str.substr(0,4);

}



string Mnemonic(string *indice, string *wordlist){
    string mnemo;
    string tmp;

    for(uint32_t i=0;i<12;i++){
        tmp = wordlist[stoi(indice[i], nullptr, 2)];
        cout<<tmp<<endl;
        mnemo += tmp; 
    }
    return mnemo;

}

void printuchar(const unsigned char* a){
    for(uint32_t i=0; i<sizeof(a)/sizeof(a[0]); i++){
        cout<<a[i]<<endl;
    }
}


struct seed {
    public:
        ifstream ls;
        string list[2048];
        string ent;
        string mnemo;
        unsigned char out[64]; //seed


        seed() : ent(tobinary(CSPRNG())) {}

    void Mnemonic_gen() {
        string hash_val = SHA256(this->ent);
        string chsum = checksum(hash_val);

        string tot = this->ent+chsum;

        string indice[12];  
        string s1;
        string *p;
        string *q = indice;
        for (uint32_t i=0;i<132;i++){
            s1="";
            s1 = tot.substr(i, 11);
            //cout<<s1<<endl;
            p = &s1;
            *q = *p;

            i += 10;
            q += 1;

        } //indice array

    

        ls.open("english.txt");
        for(uint32_t i=0;i<2048;i++){
            ls >> this->list[i];
        }
        ls.close(); //wordslist

        this->mnemo = Mnemonic(indice, this->list);
        
    }


    void seed_gen(){
        unsigned char salt[] = "mnemonic";
        PKCS5_PBKDF2_HMAC(&this->mnemo[0], this->mnemo.length(), salt, sizeof(salt)/sizeof(salt[0])-1,
                    2048, EVP_sha512(), 64, this->out); //out[512] 
        //cout<<hex_str_of_usarr(out,64)<<endl;
    }

};



struct key {
    public:
        string key_val;
        string chain_code;
    
    void cut_key(unsigned char *val) { 
        int len = 64;
        int halflen = len/2;
        vector<unsigned char> tmp(val, val+64);

        //cout<<tmp.size()<<endl;

        vector<unsigned char> L(tmp.begin(),tmp.begin()+tmp.size()/2);
        //cout<<hex_str_of_usarr(&L[0],L.size())<<endl;
        vector<unsigned char> R(tmp.begin()+tmp.size()/2,tmp.end());
        //cout<<hex_str_of_usarr(&R[0],L.size())<<endl;

        this->key_val = "0x"+hex_str_of_usarr(&L[0],L.size());
        this->chain_code = "0x"+hex_str_of_usarr(&R[0],R.size());

    }


    string public_gen(bool compress = 1) {
        string prefix;
        string pubkey;

        uint512_t val = hexstr_to_uint512(this->key_val);
        Tweedledum gen = Tweedledum::get_generator();
        Tweedledum pub = gen * val;
        
        if (compress == 1) {
            
            if(pub.y.element% 2 == 1){
                prefix ="0x03";
            }else{
                prefix ="0x02";
            }  
            pubkey = prefix + uint512_to_hex(pub.x.element);


        } else if (compress == 0) {
            prefix = "0x04";
            pubkey = prefix + uint512_to_hex(pub.x.element) + uint512_to_hex(pub.y.element);
        }

        return pubkey;
    }

    string address_gen(){
        string pub = public_gen();
        string hash_val = sha3_256(pub);
        //cout<<hash_val<<endl;

        //40 of hash_val
        string tmp = hash_val.substr(24, 40);
        //cout<<tmp<<endl;

        return "0x"+tmp;

    }

};


// int main() {
//     // seed new_seed;
//     // key master_priv;
    
//     // new_seed.Mnemonic_gen();
    
//     seed new_seed;
//     key master_priv;
    
//     new_seed.Mnemonic_gen();
//     new_seed.seed_gen(); //seed ->512

//     master_priv.cut_key(new_seed.out);
//     //cout<<master_priv.key_val<<endl;
    
//     // string tmppp = "c17075bf07a2185bee53696474075380fe7a5c0a28f2b73b5cc94cec5a271a76";
//     // string tmppp2 = "0x";
//     // string tmp = tmppp2 + tmppp;

//     // uint512_t insp(tmp);
//     // cout<<insp<<endl;

//     // string pub = master_priv.public_gen();
//     // cout<<pub<<endl;

//     cout<<master_priv.address_gen()<<endl;

//     // cout<<master_priv.key_val.size()<<endl;
//     // cout<<master_priv.chain_code.size()<<endl;
    
//     // string pub = master_priv.public_gen();
//     // cout<<pub<<endl;

//     // string hash = sha3_256(pub);
//     // cout<<hash<<endl;
    

//     // string prefix;
//     // string pubkey;

//     // uint512_t val = hexstr_to_uint512(master_priv.key_val);
//     // Tweedledum gen = Tweedledum::get_generator();
//     // Tweedledum pub = gen * val;

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
