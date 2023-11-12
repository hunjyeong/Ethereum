#include <iostream>
#include <limits>
#include <climits>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

int getPrime(){
    int srand((unsigned int)time(NULL)); //seed값으로 현재시간 부여
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

    cout << p << endl;
    
    return p;
}

int main() {
    int pp = getPrime();
    int d = rand() % pp;
    int t = rand() % pp;
    cout << pp << endl;
    cout << d << endl;
    cout << t << endl;
}
