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

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

string str_hex(const vector<unsigned char>& data) {
    ostringstream oss;

    for(const auto& byte : data) {
        oss<<hex<<static_cast<int>(byte);
    }
    string hex_str = oss.str();
    return hex_str;
}

string bytes_to_hex_string(const std::vector<uint8_t>& bytes)
{
    std::ostringstream stream;
    for (uint8_t b : bytes)
    {
        stream << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(b);
    }
    return stream.str();
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



string wordmap(string index, string *wordslist){
    string word;

    word = wordslist[stoi(index, 0, 2)];
    return word;

}

string Mnemonic(string *indice, string *wordlist){
    string mnemo;
    string tmp;

    for(uint32_t i;i<12;i++){
        tmp = wordmap(indice[i], wordlist);
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

class seed {
    public:
        ifstream ls;
        string list[2048];
        string ent;
        string mnemo;
        unsigned char out[512]; //seed


    seed() : ent(tobinary(CSPRNG())) {}

    void Mnemonic_gen() {
        string hash_val = SHA256(this->ent);
        string chsum = checksum(hash_val);

        string tot = this->ent+chsum;

        string indice[12];  
        string s1;
        string *p;
        string *q = indice;

        for (uint32_t i;i<132;i++){
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

        this->mnemo = Mnemonic(indice, list);
    }

    void seed_gen(){
        unsigned char salt[] = "mnemonic";
        PKCS5_PBKDF2_HMAC(&this->mnemo[0], this->mnemo.length(), salt, sizeof(salt)/sizeof(salt[0])-1,
                    2048, EVP_sha512(), 64, this->out);
    }

};

class key {
    public:
        string key_val;
        string chain_code;
    
    void cut_key(unsigned char *val) {
        int len = sizeof(val)/sizeof(val[0]);
        int halflen = len/2;
        vector<unsigned char> L(val, val+halflen);
        vector<unsigned char> R(val+halflen, val+len);

        this->key_val = str_hex(L);
        this->chain_code = str_hex(R);

    }


    string public_gen(bool compress = 1) {
        string prefix;
        string pubkey;

        uint512_t val = hexstr_to_uint512(this->key_val);
        Tweedledum gen = Tweedledum::get_generator();
        Tweedledum pub = gen * val;
        
        if (pub.is_on_curve() == 0) {
            throw runtime_error("Not on the coruve");
        };
        
        if (compress == 1) {
            
            if(pub.y.element% 2 == 1){
                prefix ="0x03";
            }else{
                prefix ="0x02";
            }
            
            pubkey = prefix + boost::lexical_cast<std::string>(pub.x.element);

        } else if (compress == 0) {
            prefix = "0x04";
            pubkey = prefix + boost::lexical_cast<std::string>(pub.x.element) + boost::lexical_cast<std::string>(pub.y.element);
        }

        pubkey;
    }

    string address_gen(string pubkey){
        string hash_val = sha3_256(pubkey);
    }

};


int main() {
    seed new_seed;
    key master_priv;
    
    new_seed.Mnemonic_gen();
    new_seed.seed_gen(); //seed 

    master_priv.cut_key(new_seed.out);
    
    string pubk = master_priv.public_gen(0);
    string pubk2 = master_priv.public_gen();
    
    cout<<pubk<<endl;
    cout<<pubk2<<endl;
    
    


}
