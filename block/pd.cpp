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
    void committer(int Com, int g, int h, int x, int r, int p);
    /*void verifier();
    void prover();*/

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

void PCS::committer(int Com, int g, int h, int x, int r, int p){

    int g1 = pow(g, x);
    g1 = g1 % p;
    int h1 = pow(h, r);
    h1 = h1 % p;
    Com = (g1 * h1) % p;
    cout << "Com = " << Com << endl;
}


int main() {
    int p = getPrime();
    cout << "p = " << p << endl; // prime number
    
    // Set values for g, h, p, x, and r
    int Com;
    int g = 2;  // generator
    int h = 3;  // another generator
    int x = rand() % p; //message, 0~q-1 중에 한가지 수 출력, // random value for x
    int r = rand() % p; //randomness, random value for r
    
    PCS co;

    co.committer(Com, g, h, x, r, p);

    cout << "g = " << g << endl;
    cout << "h = " << h << endl;
    cout << "x = " << x << endl;
    cout << "r = " << r << endl;

   
}
