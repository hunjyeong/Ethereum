#ifndef ECDSA___H_
#define ECDSA___H_
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
#include <chrono>
#include <fstream>
#include "EEA.hpp"
#include "finitefield.hpp"
#include "ellipticcurve.hpp"
#include "seed1.hpp"
#include "ECDSA_.cpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

struct Sig;
Sig Sig_Gen(const key& privateKey, const string& message);

bool Sig_Verify(const Tweedledum& pubkey, const string& message, const Sig& sig);

#endif