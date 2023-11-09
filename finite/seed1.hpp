#ifndef SEED1_H__
#define SEED1_H__

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
#include "seed1.cpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;
string hex_str_of_usarr(unsigned char* in, size_t len);
string uint512_to_string(uint512_t num);
string str_hex(const vector<unsigned char>& data);
string bytes_to_hex_string(const std::vector<uint8_t>& bytes);
string uint512_to_hex(uint512_t num);
string sha3_256(const std::string& input);
string SHA256(const string strIn);
uint128_t CSPRNG();
string tobinary(uint128_t n);
string checksum(string n);
string Mnemonic(string *indice, string *wordlist);
void printuchar(const unsigned char* a);

struct key;
struct seed;

#endif
