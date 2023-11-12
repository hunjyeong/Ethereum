#ifndef EEA_H__
#define EEA_H__

#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include "EEA.cpp"

using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

uint512_t extended_gcd(uint512_t a, uint512_t b, int512_t& x, int512_t& y);  


uint512_t mod_inverse(uint512_t a, uint512_t m);


#endif