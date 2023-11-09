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
#include "ECDSA_.hpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

class serial{
    public:
        string origin;
        string res;

    // string RLP(){
    //     // ifstream ls;
    //     // ls.open("english.txt");
    //     // for(uint32_t i=0;i<2048;i++){
    //     //     ls >> this->list[i];
    //     // }
    //     // ls.close(); //wordslist

    //     char a[this->origin.size()];
    //     strcpy(a, this->origin.c_str());

    //     bool empty_check = 0;
    //     if(&a[0] == ""){
    //         empty_check = 1;
    //     }
        
    //     if (empty_check) {
    //         throw std::runtime_error("original string is empty");
    //     }
    //     else{
    //         if(this->origin.size() == 1){
    //             char tmp = a[0];
    //             cout<<tmp<<endl;
    //         }
    //     }

    // }

};



uint32_t byteofstring(string content){
    return (sizeof(content)/32);

}

// string bin_to_hex(string bin){
//     string tmp;
//     uint128_t bin_size = bin.size();
//     uint128_t num = bin_size / 8;
    
//     if((bin_size % 8) != 0){
//         num += 1;
//     };

//     for(uint128_t i=0; i<num; i++){
//         tmp = bin.substr(bin.length() - 8);
        
//     }

//     str_hex()

// }


int main() {
    string txt = "abc";
    //cout<<byte_of_string(txt)<<endl;
    serial a;
    a.origin = txt;

    uint512_t tmp;

    string prefix;
    char A[a.origin.size()];
    strcpy(A, a.origin.c_str());

    cout<<A<<endl;

    bool empty_check = 0;
    if(&A[0] == ""){
            empty_check = 1;
    }

    cout<<empty_check<<endl;
    
    if (empty_check) {
            throw std::runtime_error("original string is empty");
        }
    else{
        if(a.origin.size() == 1){
            tmp = int(A[0]);
            a.res = "0x" + uint512_to_hex(tmp);
        }
        else if(a.origin.size() <= 55){ //2~55 bytes
            prefix = "0x80";
            uint512_t origin_size = a.origin.size();
            tmp = hexstr_to_uint512(prefix);
            tmp += origin_size;
            
            string tmp1;
            tmp1 = uint512_to_hex(tmp);
            a.res = "0x" + tmp1;

            for(uint64_t i=0;i<origin_size;i++){
                tmp = int(A[i]);
                a.res += uint512_to_hex(tmp);
            }
        }
        else if(a.origin.size() > 55){
            prefix = "0xb7";
            uint128_t origin_size = a.origin.size();
            string bin = tobinary(origin_size); //binary representation of origin_size
            
            uint512_t len = bin.size() / 8;
            if((bin.size()/8) % 8 != 0){
                len += 1;
            };
            //byte num of binary

            tmp = hexstr_to_uint512(prefix);
            tmp += len; //for prefix

            string tmp1;
            tmp1 = uint512_to_hex(tmp);
            a.res = "0x" + tmp1; //1st prefix


        }
        
    }
    // cout<<tmp<<endl;
    // cout<<a.res<<endl;

    string bin = tobinary(1000);
    cout<<bin<<endl;

}
