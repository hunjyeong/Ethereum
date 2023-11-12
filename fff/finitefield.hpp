#ifndef FINITEFIELD_H__
#define FINITEFIELD_H__

#include <iostream>
#include <openssl/sha.h>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include "EEA.hpp"
#include "finitefield.cpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

uint512_t hexstr_to_uint512(const std::string& hex_str);

class Fp;
class Fq;

#endif