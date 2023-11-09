#ifndef ELLIPTICCURVE_H__
#define ELLIPTICCURVE_H__

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
#include "finitefield.hpp"
#include "ellipticcurve.cpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;


class Tweedledum;
uint512_t hexstr_to_uint512(const std::string& hex_str);


#endif