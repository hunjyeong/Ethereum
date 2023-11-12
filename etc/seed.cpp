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
#include <openssl/evp.h>

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

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




int main() {

    string list[2048];
    ifstream ls;

    uint128_t ent;
    ent = CSPRNG(); //uint128
    //cout<<sizeof(ent)<<endl;

    string entrophy;
    entrophy = tobinary(ent); //128bits
    //cout<<entrophy<<endl;

    //checksum

    string hash_val = SHA256(entrophy);
    // cout<<hash_val<<endl;
    string chsum = checksum(hash_val);

    string tot = entrophy+chsum;
    //cout<<tot<<endl;

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

    // cout<<indice[0]<<endl;
    // cout<<stoi(indice[0], 0, 2)<<endl;


    ls.open("english.txt");
    for(uint32_t i=0;i<2048;i++){
        ls >> list[i];
    }
    ls.close(); //wordslist

    unsigned char out[512];
    string mnemo = Mnemonic(indice, list);
    unsigned char salt[] = "mnemonic";
    PKCS5_PBKDF2_HMAC(&mnemo[0],mnemo.length(), salt, sizeof(salt)/sizeof(salt[0])-1,
                    2048, EVP_sha512(), 512, out);

    //printuchar(out);

    for(uint32_t i = 0; i < 512; i++){
        printf("%.2x", out[i]);}

}
