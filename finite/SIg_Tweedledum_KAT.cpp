#include <chrono>
#include <fstream>
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

std::string RandHexStr() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::ostringstream oss;
    for (int i = 0; i < 32; ++i) {
        int randomValue = dis(gen);
        oss << std::hex << randomValue;
    }

    return "0x"+oss.str();
}


void Performance_Tweedledum(int numOfTrials) {

    std::ofstream csvFile;
    csvFile.open("Performance_Tweedledum.csv");
    csvFile << "MessageSize(byte),Sig_Gen(ms),Sig_Verify(ms)"<<endl;

    for (int size = 1; size <= 100; size++) {
        double averageSigGenTime = 0.0;
        double averageSigVerTime = 0.0;

        for (int i = 0; i < numOfTrials; i++) {

            std::string message;
            for(int j = 0; j < size; j++) {
                message += 'a' + rand() % 26;
            }

            
            key privateKey;
            std::string priv = RandHexStr();
            privateKey.key_val = priv;
            Tweedledum publicKey = (Tweedledum::G)*Fq(priv); 

            auto startSigGen = std::chrono::high_resolution_clock::now();
            Sig sig = Sig_Gen(privateKey, message);
            auto endSigGen = std::chrono::high_resolution_clock::now();
            double durationSigGen = std::chrono::duration<double, std::milli>(endSigGen - startSigGen).count();
            averageSigGenTime += durationSigGen;


            auto startSigVer = std::chrono::high_resolution_clock::now();
            bool flag = Sig_Verify(publicKey, message, sig);
            auto endSigVer = std::chrono::high_resolution_clock::now();
            double durationSigVer = std::chrono::duration<double, std::milli>(endSigVer - startSigVer).count();
            averageSigVerTime += durationSigVer;
        }

     
        averageSigGenTime /= numOfTrials;
        averageSigVerTime /= numOfTrials;

        csvFile << size << "," << averageSigGenTime << "," << averageSigVerTime << endl;
    }

    csvFile.close();
}

int main() {
    
    Performance_Tweedledum(100);

    return 0;
}
