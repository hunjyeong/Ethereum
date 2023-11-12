// #define secp256k

// #include <chrono>
// #include <fstream>
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
// #include "seed2.hpp"
// #include "ECDSA_secp256k1.hpp"

// std::string RandHexStr() {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dis(0, 15);

//     std::ostringstream oss;
//     for (int i = 0; i < 32; ++i) {
//         int randomValue = dis(gen);
//         oss << std::hex << randomValue;
//     }

//     return "0x"+oss.str();
// }


// void Performance_secp256k1(int numOfTrials) {

//     std::ofstream csvFile;
//     csvFile.open("Performance_secp256k1.csv");
//     csvFile << "MessageSize(byte),Sig_Gen(ms),Sig_Verify(ms)"<<endl;

//     for (int size = 1; size <= 100; size++) {
//         double averageSigGenTime = 0.0;
//         double averageSigVerTime = 0.0;

//         for (int i = 0; i < numOfTrials; i++) {

//             std::string message;
//             for(int j = 0; j < size; j++) {
//                 message += 'a' + rand() % 26;
//             }

            
//             key privateKey;
//             std::string priv = RandHexStr();
//             privateKey.key_val = priv;
//             secp256k1 publicKey = (secp256k1::G)*Fq(priv); 

//             auto startSigGen = std::chrono::high_resolution_clock::now();
//             Sig sig = Sig_Gen(privateKey, message);
//             auto endSigGen = std::chrono::high_resolution_clock::now();
//             double durationSigGen = std::chrono::duration<double, std::milli>(endSigGen - startSigGen).count();
//             averageSigGenTime += durationSigGen;


//             auto startSigVer = std::chrono::high_resolution_clock::now();
//             bool flag = Sig_Verify(publicKey, message, sig);
//             auto endSigVer = std::chrono::high_resolution_clock::now();
//             double durationSigVer = std::chrono::duration<double, std::milli>(endSigVer - startSigVer).count();
//             averageSigVerTime += durationSigVer;
//         }

     
//         averageSigGenTime /= numOfTrials;
//         averageSigVerTime /= numOfTrials;

//         csvFile << size << "," << averageSigGenTime << "," << averageSigVerTime << endl;
//     }

//     csvFile.close();
// }

// std::string RandHexStr(int numBytes) {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dis(0, 15);

//     std::ostringstream oss;
//     for (int i = 0; i < numBytes; ++i) {
//         int randomValue = dis(gen);
//         oss << std::hex << randomValue;
//     }

//     return "0x"+oss.str();
// }

// void Performance_secp256k1_key(int numOfTrials) {

//     std::ofstream csvFile;
//     csvFile.open("Performance_secp256k1_key.csv");
//     csvFile << "PrivateKeySize(bits),Sig_Gen(ms),Sig_Verify(ms)"<<endl;

//     int messageSize = 100; 

//     std::vector<int> privKeySizes = {1, 2, 4, 8, 16, 32}; 
//     for (int size : privKeySizes) {
//         double averageSigGenTime = 0.0;
//         double averageSigVerTime = 0.0;

//         for (int i = 0; i < numOfTrials; i++) {

//             std::string message;
//             for(int j = 0; j < messageSize; j++) {
//                 message += 'a' + rand() % 26;
//             }

//             key privateKey;
//             std::string priv = RandHexStr(size);
//             privateKey.key_val = priv;
//             secp256k1 publicKey = (secp256k1::G)*Fq(priv); 

//             auto startSigGen = std::chrono::high_resolution_clock::now();
//             Sig sig = Sig_Gen(privateKey, message);
//             auto endSigGen = std::chrono::high_resolution_clock::now();
//             double durationSigGen = std::chrono::duration<double, std::milli>(endSigGen - startSigGen).count();
//             averageSigGenTime += durationSigGen;


//             auto startSigVer = std::chrono::high_resolution_clock::now();
//             bool flag = Sig_Verify(publicKey, message, sig);
//             auto endSigVer = std::chrono::high_resolution_clock::now();
//             double durationSigVer = std::chrono::duration<double, std::milli>(endSigVer - startSigVer).count();
//             averageSigVerTime += durationSigVer;
//         }

     
//         averageSigGenTime /= numOfTrials;
//         averageSigVerTime /= numOfTrials;

//         csvFile << size*8 << "," << averageSigGenTime << "," << averageSigVerTime << endl; 
//     }

//     csvFile.close();
// }

// int main() {
    
//     //Performance_secp256k1(100);
//     Performance_secp256k1_key(100);

//     return 0;
// }