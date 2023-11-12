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

struct transaction {
    public: 
        uint32_t nonce; //40years 1minute
        uint64_t gasPrice;
        uint64_t gasLimit;
        string to;
        uint64_t value;
        string data;
        string tx_v;
        string tx_r;
        string tx_s;

    
    void Gen(){

        cout<<"to : ";
        cin >> this->to;
        cout<<"gasPrice : ";
        cin >> this->gasPrice;
        cout<<"gasLimit : ";
        cin >> this->gasLimit;
        cout<<"value : ";
        cin >> this->value;
        cout<<"data : ";
        cin >> this->data;

        this->nonce = 0; //MPT전까지는 0으로 설정
        this->tx_v = "0";
        this->tx_r = "0";
        this->tx_s = "0";

    }

    string TXserial(){
    
        //concat
        string non = to_string(this->nonce);
        string gp = to_string(this->gasPrice);
        string gl = to_string(this->gasLimit);
        string val = to_string(this->value);

        string serial = non + gp + gl + val + this->data + this->to + this->tx_v + this->tx_r + this->tx_s;

        return serial;
    }

};

struct PW{
    string pass;

    void new_account(){
        seed new_seed;
        key master_priv;
        string pw;

        cout<<"password : ";
        cin>>pw;
    
        this->pass = pw;

        new_seed.Mnemonic_gen();
        new_seed.seed_gen(); //seed ->512

        master_priv.cut_key(new_seed.out);
        cout<<master_priv.key_val<<endl;
        keystore_gen(master_priv);

    }

    void keystore_gen(key privatekey){
        ofstream of;
        of.open("keyfile.txt");
        
        string keyval = privatekey.key_val;
        of.write(keyval.c_str(), keyval.size());

        of.close();

    }
};


int main(){
    ifstream is;

    PW password;
    password.new_account();

    transaction tx;
    Sig new_sign;

    tx.Gen();
    //cout<<Tx1.gasLimit<<endl;
    //cout<<tx.gasPrice<<endl;
    string txst = tx.TXserial();
    // //Sig Sig_Gen(const key& privateKey, const std::string& message)
    is.open("keyfile.txt");
    string privatekey;
    is >> privatekey;
    //cout<<privatekey<<endl;
    key pr;
    pr.key_val = privatekey;
    new_sign = Sig_Gen(pr, txst);

    //bool Sig_Verify(const Tweedledum& pubkey, const string& message, const Sig& sig)
    

    
}

// void Gen(transaction tx){

//         cout<<"to : ";
//         cin >> tx.to;
//         cout<<"gasPrice : ";
//         cin >> tx.gasPrice;
//         cout<<"gasLimit : ";
//         cin >> tx.gasLimit;
//         cout<<"value : ";
//         cin >> tx.value;
//         cout<<"data : ";
//         cin >> tx.data;

//         tx.nonce = 0; //MPT전까지는 0으로 설정
//         tx.tx_v = "0";
//         tx.tx_r = "0";
//         tx.tx_s = "0";

//     }

// string TXserial(transaction tx){
    
//         //concat
//         string non = to_string(tx.nonce);
//         string gp = to_string(tx.gasPrice);
//         string gl = to_string(tx.gasLimit);
//         string val = to_string(tx.value);

//         string serial = non + gp + gl + val + tx.data + tx.to + tx.tx_v + tx.tx_r + tx.tx_s;

//         return serial;
// }

// int main(){
    
//     transaction Tx1;
//     Sig new_sign;


//     Tx1.Gen();
//     cout<<Tx1.gasLimit<<endl;
//     cout<<Tx1.gasPrice<<endl;
    
//     cout<<Tx1.TXserial()<<endl;

//     //Sig Sig_Gen(const key& privateKey, const std::string& message)
//     // new_sign = Sig_Gen();



// }
