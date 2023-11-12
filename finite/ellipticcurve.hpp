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

// Tweedledum operator*(const Fq& k, const Tweedledum& P);

// Tweedledum operator*(int k, const Tweedledum& P);

// Tweedledum operator*(const Tweedledum& P, int k);

#endif