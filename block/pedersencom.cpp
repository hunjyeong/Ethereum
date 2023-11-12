#include <mcl/bls12_381.hpp>
#include <iostream>
#include <limits>
#include <climits>
#include <bitset>
#include <cmath>
#include "primitiveroots.hpp"
#include "ntt.hpp"
#include "polyonfr.cpp"
#include <cstdlib>
#include <ctime>
#include <list>


using namespace std;


class PCS {

public:
    void committer(mcl::bn::G1 Com, mcl::bn::G1 g, mcl::bn::G1 h, int x, int r, int p);
    /*void verifier();
    void prover(G1, G1, int m, int r);*/

};


int getPrime(){
    srand((unsigned int)time(NULL)); //seed값으로 현재시간 부여
    int i;
    int p=0;

    while(p == 0){
        int n = rand(); 
        for (i=2; i<= n/2; i++){
            if (n%i == 0){
                break;//새로 뽑아야함
            }
            if (i == n/2){
                p = n;
            }
        }
    }
    
    return p;
}

void PCS::committer(mcl::bn::G1 Com, mcl::bn::G1 g, mcl::bn::G1 h, int x, int r, int p){
    
    G1 g;
    G1 h;

    mapToG1(g, )
    // Set values for g, h, p, x, r
    //Fp // Fp : Zp
    //Fr x;// Fr : Zq (Z|G1|) message
    //vector<Fr> x;

    ::pow(cg, g, x, p); // g^x (mod p)
    g1 = g1 % p;
    int h1 = pow(h, r); // h^r (mod p)
    h1 = h1 % p;
    Com = (g1 * h1) % p; // Com = g^x * h^r (mod p)--> x*g + r*h
    cout << "Com = " << Com << endl; 

}


int main() {
    int p = getPrime();
    cout << "p = " << p << endl;

    /*int i, q = 0, t = 1;
    while(q == 0){
        int qq = t*p + 1;
        for (i=2; i<= qq/2; i++){
            if (qq%i == 0){
                break;//새로 뽑아야함
            }
            if (i == qq/2){
                q = qq;
            }
        }
        t += 1;
    }
    cout << "q = ";
    cout << q << endl;
    cout << "t = ";
    cout << t << endl;*/

    /*//Compute elements of G = {i^t mod q | i in Z_q*}
    list<int> G;
    for (i=1; i < q; i++) {// Z_q*
        G.push_back(pow(i, t) % q); //원소 추가
    }*/
    

    // Set values for g, h, p, x, r
    G1 Com;
    G1 g; // generator
    G1 h; // another generator
    int x = rand() % p; //message, 0~q-1 중에 한가지 수 출력, // random value for x
    int r = rand() % p; //randomness, random value for r
    
    r.set By spr

    //Fp // Fp : Zp
    Fr x;// Fr : Zq


    PCS co;

    co.committer(Com, g, h, x, r, p);

    cout << "g = " << g << endl;
    cout << "h = " << h << endl;
    cout << "x = " << x << endl;
    cout << "r = " << r << endl;
}
