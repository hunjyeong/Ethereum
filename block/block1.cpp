#include <iostream>
#include <openssl/sha.h>
#include <random>
//#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/algorithm/hex.hpp>
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
// #include <boost/lexical_cast.hpp>
// #include "EEA.hpp"
// #include "finitefield.hpp"
// #include "ellipticcurve.hpp"
// #include "seed1.hpp"
// #include "ECDSA_.hpp"

using namespace::std;
//using namespace boost::multiprecision;
typedef unsigned char BYTE;

string sha256(const string str){
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
};

string block_hash(string parentHash, string coinbase, string transactionRoot, string mixHash, string nonce,
                            uint64_t difficulty, uint64_t number, uint64_t gasLimit, uint64_t gasUsed, uint64_t timestamp){

    string header_data = parentHash + coinbase + transactionRoot + mixHash + nonce +
                        to_string(difficulty) + to_string(number) + to_string(gasLimit) + to_string(gasUsed) + to_string(timestamp);

    string header_hash = sha256(header_data);
    //string header_hash = sha3_256(header_data);

    cout << "Block Hash: " << header_hash << endl;

    return header_hash;

};

string block_pow(string parentHash, string coinbase, string transactionRoot, string mixHash, string nonce,
                            uint64_t difficulty, uint64_t number, uint64_t gasLimit, uint64_t gasUsed, uint64_t timestamp){

    string hash_value;
    string update_nonce = nonce;


    //cout << "nonce(base) : " << nonce << endl;

    hash_value = block_hash(parentHash, coinbase, transactionRoot, mixHash, nonce, difficulty, number, gasLimit, gasUsed, timestamp);

    uint64_t nonce_uint;
    std::stringstream sh;
    sh << std::hex << nonce;
    sh >> nonce_uint;   // hex값을 unit64로 바꿈 (nonce -> 8 bytes)
    //cout << "nonce(int) : " << nonce_uint << endl;


    // while (hexstr_to_uint512(hash_value) > hexstr_to_uint512(mixHash)){

    //     nonce_uint += 1; //nonce 값 1씩 증가 시킴 
    //     update_nonce = "";

    //     std::stringstream ss_nonce;
    //     ss_nonce << std::hex << nonce_uint; // nonce값을 다시 hex로 바꿈
    //     update_nonce = ss_nonce.str();
    //     //cout << "nonce(hex) : " << nonce_uint << endl;

    //     hash_value = block_hash(parentHash, coinbase, transactionRoot, mixHash, update_nonce, difficulty, number, gasLimit, gasUsed, timestamp);
    // }

    return update_nonce;

};



struct block_header {

    public:

        string parentHash = "0"; //이전 block의 hash값
        //string uncleHash;
        string coinbase; //채굴 후 보상 받을 주소
        //string stateRoot;
        string transactionRoot;
        //string receiptRoot;
        //string bloom;
        //string extraData;

        string mixHash = "0x"; // nonce와 pow를 하는데 이용되는 256 bits hash : 32 bytes => 64개
        string nonce = "0x"; // pow시 이용되는 64 bits hash : 8 bytes => 16 개
        uint64_t difficulty;

        uint64_t number = 0;
        uint64_t gasLimit;
        uint64_t gasUsed = 0;
        uint64_t timestamp;

        void block_gen();
        

    void header_Gen(string previous_hash, string transaction_hash, uint64_t num, uint64_t gasUs){

        //this->parentHash = block_hash(this->parentHash, this->coinbase, this->transactionRoot, this->mixHash.substr(2), this->nonce.substr(2), this->difficulty, this->number, this->gasLimit, this->gasUsed, this->timestamp);
        this->parentHash = previous_hash;
        cout<<"coinbase : ";
        cin >> this->coinbase;
        this->transactionRoot = transaction_hash;

        this->mixHash = "0x14bca881b07a6a09f83b130798072441705d9a665c5ac8bdf2f39a3cdf3bee29";
        //this->mixHash = mixHash.substr(2);
        this->nonce = "0xd3e8412b4fb3d26b";
        //this->nonce = nonce.substr(2);

        this->difficulty = 3324092183262715;
        this->number = num;
        this->gasLimit = 10000;
        this->gasUsed += gasUs;
        this->timestamp = static_cast<uint32_t>(time(nullptr));

        //cout << "original nonce :" << this->nonce << endl;

        this->nonce = block_pow(this->parentHash, this->coinbase, this->transactionRoot, this->mixHash.substr(2), this->nonce.substr(2), this->difficulty, this->number, this->gasLimit, this->gasUsed, this->timestamp);
        
        this->nonce = this->nonce.insert(0, "0x");

        cout << "update nonce :" << this->nonce << endl;
        //cout << timestamp << endl;

        block_gen();

    }

};

void block_header::block_gen(){


    ofstream ofs;
    ofs.open("block.txt");

    ofs << parentHash << endl;
    ofs << coinbase << endl;
    ofs << transactionRoot << endl;
    ofs << mixHash << endl;
    ofs << nonce << endl;

    ofs << difficulty << endl;
    number = 1;
    ofs << number << endl;
    ofs << gasLimit << endl;
    ofs << gasUsed << endl;
    ofs << timestamp << endl;

    // string keyval = ;
    // ofs.write();

    ofs.close();

}


int main() {

    ifstream ifs;

    block_header blockHeader;

    string transaction_hash = "abcdef";


    ifs.open("block.txt");
    string blocktxt[5];
    uint64_t blocktxt_uint[5];
    for (int i = 0; i < 5; i++){
        ifs >> blocktxt[i];    //파일 읽기
        //cout << blocktxt[i] << endl;
    }
    for (int i = 0; i < 5; i++){
        ifs >> blocktxt_uint[i];    //파일 읽기
        //cout << blocktxt_uint[i] << endl;
    }

    ifs.close();

    uint64_t tx_gas = 100;

    uint64_t num = blocktxt_uint[1] + 1;
    uint64_t gasUs = blocktxt_uint[3] + tx_gas;

    string pre_hash = block_hash(blocktxt[0], blocktxt[1], blocktxt[2], blocktxt[3].substr(2), blocktxt[4].substr(2), 
                                    blocktxt_uint[0], blocktxt_uint[1], blocktxt_uint[2], blocktxt_uint[3], blocktxt_uint[4]);

    blockHeader.header_Gen(pre_hash, transaction_hash, num, gasUs);


}