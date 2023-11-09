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
        string tx_v; //pubkey_info
        uint512_t tx_r;
        uint512_t tx_s;

    
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
        this->tx_r = 0;
        this->tx_s = 0;

    }

    string TXserial(){
    
        //concat
        string non = to_string(this->nonce);
        string gp = to_string(this->gasPrice);
        string gl = to_string(this->gasLimit);
        string val = to_string(this->value);
        string r = uint512_to_string(this->tx_r);
        string s = uint512_to_string(this->tx_s);

        string serial = non + gp + gl + val + this->data + this->to + this->tx_v + r + s;

        return serial;
    }

    // void pubkey_call(){
    //     Fq r = this->tx_r;
    //     Fq s = this->tx_s;
    //     Fq inv_r = Fq(1)/r;

    //     Fq sq_y = r * r * r + Fq(5);
    //     Fq y = sq_y.sqrt();

    //     Tweedledum tmp;
    //     //tmp.x = r;
    //     //tmp.y = y;

    //     cout<<r<<endl;
    //     cout<<y<<endl;
    // }
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


// int main(){
//     ifstream is;

//     PW password;
//     password.new_account();

//     transaction tx;
//     Sig new_sign;

//     tx.Gen();
//     //cout<<Tx1.gasLimit<<endl;
//     //cout<<tx.gasPrice<<endl;
//     string txst = tx.TXserial();
//     // //Sig Sig_Gen(const key& privateKey, const std::string& message)
//     is.open("keyfile.txt");
//     string privatekey;
//     is >> privatekey;
//     //cout<<privatekey<<endl;
//     key pr;
//     pr.key_val = privatekey;
    
//     // Tweedledum G = Tweedledum::get_generator();
//     // Fq d(pr.key_val);
//     // Tweedledum pubkey = G*d;
//     // cout<<pubkey<<endl;

//     new_sign = Sig_Gen(pr, txst);

//     //transaction including r, s
//     tx.tx_r = new_sign.r.element;
//     tx.tx_s = new_sign.s.element;

//     //for transaction's contents
//     string pub_info1 = uint512_to_string(new_sign.v.G_x.element);
//     string pub_info2 = uint512_to_string(new_sign.v.G_y.element);
//     tx.tx_v = pub_info1 + pub_info2; 

//     // cout<<"pubinfo"<<endl;
//     // cout<<new_sign.v<<endl;
//     // tx.tx_v = new_sign.v;

//     //bool Sig_Verify(const Tweedledum& pubkey, const string& message, const Sig& sig)
//     bool res = Sig_Verify(new_sign.v, txst, new_sign);
//     cout<<res<<endl;

//     // cout<<tx.tx_r<<endl;
//     // cout<<tx.tx_s<<endl;
    
//     // cout<<"----"<<endl;
//     //tx.pubkey_call();
//     // Fq r = tx.tx_r;
//     // Fq s = tx.tx_s;
//     // Fq inv_r = Fq(1)/r;
//     // Tweedledum tmp;
    
//     // for(uint512_t i=0;i<r.modul;i++){
//     //     tmp = G * i;
//     //     if(tmp.G_x.element == r.element){
//     //         break;
//     //     }
//     // }

//     // cout<<tmp<<endl;
    

//     // cout<<r<<endl;
//     // cout<<s<<endl;

//     //gen pubkey


//     //bool Sig_Verify(const Tweedledum& pubkey, const string& message, const Sig& sig)
    

    
// }
