// #include <iostream>
// #include <openssl/sha.h>
// #include <random>
// #include <boost/multiprecision/cpp_int.hpp>
// #include <boost/algorithm/hex.hpp>
// #include <fstream>
// #include <algorithm>
// #include <vector>
// #include <string>
// #include <numeric>
// #include <math.h>
// #include <iomanip>
// #include <sstream>
// #include <locale>
// #include <openssl/sha.h>
// #include <openssl/evp.h>
// #include <openssl/bio.h>
// #include <boost/lexical_cast.hpp>
// #include "EEA.hpp"
// #include "finitefield.hpp"
// #include "ellipticcurve.hpp"
// #include "seed1.hpp"
// //#include "ECDSA_.hpp"
// //#include "transaction.hpp"

// using namespace::std;
// using namespace boost::multiprecision;
// typedef unsigned char BYTE;

// struct PW{
//     string pass;

//     void new_account(){
//         seed new_seed;
//         key master_priv;
//         string pw;

//         cout<<"password : ";
//         cin>>pw;
    
//         this->pass = pw;

//         new_seed.Mnemonic_gen();
//         new_seed.seed_gen(); //seed ->512

//         master_priv.cut_key(new_seed.out);
//         cout<<master_priv.key_val<<endl;
//         keystore_gen(master_priv);

//     }

//     void keystore_gen(key privatekey){
//         ofstream of;
//         of.open("keyfile.txt");
        
//         string keyval = privatekey.key_val;
//         of.write(keyval.c_str(), keyval.size());

//         of.close();

//     }
// };


// int main(){
//     ifstream is;

//     PW password;
//     password.new_account();

//     // transaction tx;
//     // Sig new_sign;

//     //Gen(tx);
//     //cout<<Tx1.gasLimit<<endl;
//     //cout<<tx.gasPrice<<endl;
//     // string txst = tx.TXserial();
//     // //Sig Sig_Gen(const key& privateKey, const std::string& message)
//     // is.open("keyfile.txt");
//     // string privatekey;
//     // is >> privatekey;
//     // cout<<privatekey<<endl;

//     //new_sign = Sig_Gen();

    
// }