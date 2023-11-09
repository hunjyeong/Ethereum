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

string pubkey(const uint512_t& num) {
    Tweedledum keytimesG;
    keytimesG = Tweedledum::get_generator()*num;
    string prefix;
    if(keytimesG.y.element%2==1){
        prefix ="03";
    }else{
        prefix ="02";
    }

    stringstream ss;
    ss <<prefix<<setfill('0')<<setw(32)<<hex << keytimesG.x.element;

    string hex_string = ss.str();
    for (char& c : hex_string) {
        c = tolower(c, std::locale());
    }

    return hex_string;
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




int main() {

    string list[2048];
    ifstream ls;

    uint128_t ent;
    ent = CSPRNG(); //uint128
    cout<<"random value"<<endl;
    cout<<ent<<endl;
    cout<<"===================="<<endl;

    string entrophy;
    entrophy = tobinary(ent); //128bits
    cout<<"random value_binary"<<endl;
    cout<<entrophy<<endl;
    cout<<"===================="<<endl;

    //checksum

    string hash_val = SHA256(entrophy);
    // cout<<hash_val<<endl;
    string chsum = checksum(hash_val);
    cout<<"checksum"<<endl;
    cout<<chsum<<endl;
    cout<<"===================="<<endl;

    string tot = entrophy+chsum;
    cout<<"random+checksum"<<endl;
    cout<<tot<<endl;
    cout<<"===================="<<endl;

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
    PKCS5_PBKDF2_HMAC(&mnemo[0], mnemo.length(), salt, sizeof(salt)/sizeof(salt[0])-1,
                    2048, EVP_sha512(), 64, out);

    //printuchar(out);
    cout<<"=========="<<endl;
    cout<<"seed"<<endl;
    for(uint32_t i = 0; i < 512; i++){
        printf("%x", out[i]);}
    
    printf("\n");
    //private_key (master private||chaincode)
    cout<<"=========="<<endl;
    
    int len = sizeof(out)/sizeof(out[0]);
    int halflen = len/2;
    vector<unsigned char> L(out, out+halflen);
    vector<unsigned char> R(out+halflen, out+len);

    string hexout = str_hex(L);
    cout<<"L: "<<hexout<<endl;
    string hexout2 = str_hex(R);
    cout<<"R: "<<hexout2<<endl;

    //publickey
    uint512_t priv = hexstr_to_uint512(hexout);
    Tweedledum gen = Tweedledum::get_generator();
    Tweedledum pub = gen * priv;
    string prefix;
    if(pub.y.element%2==1){
        prefix ="03";
    }else{
        prefix ="02";
    }
    cout<<"public key: "<<prefix<<setfill('0')<<setw(32)<<hex<<pub.x<<endl;
    cout<<"priv*G is on curve: "<<pub.is_on_curve()<<endl;

    cout<<"pubkey: "<<pubkey(priv)<<endl;

    
}