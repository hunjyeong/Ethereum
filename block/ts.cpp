#include <mcl/bls12_381.hpp>
#include <iostream>
#include <limits>
#include <climits>
#include <bitset>
#include <cmath>
#include "primitiveroots.hpp"
#include "ntt.hpp"
#include "polyonfr.cpp"


int main(){

    initPairing(mcl::BLS12_381);

    FrVec f = {1,1};
    
    FrVec g = PolyMul(f,f);

    put("g",g);

    Fr s;
    s.setByCSPRNG();
    Fr v=0;
    Fr v2=0;
    Fr tmp;
    Fr tmps;

    for(int i=0;i<f.size();i++){

        Fr::pow(tmps,s,i);
        tmp = tmps*f[i];
        v=v+tmp;
    }

    for(int i=0;i<g.size();i++){
        Fr::pow(tmps,s,i);
        tmp = tmps*g[i];
        v2=v2+tmp; 
    }

    GT res;
    GT res2;
    G1 g1;
    G2 g2;

    hashAndMapToG1(g1,"g1");
    hashAndMapToG2(g2,"g2");

    pairing(res,g1*v,g2*v);
    pairing(res2,g1,g2*v2);


    cout<<(res==res2)<<endl;



}