#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#include <numeric>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>    //crypto.h, evp.h 차이


using namespace std;
typedef unsigned char BYTE; // 1 byte (8 bit) 0~255, BYTE는 char형의 배열

string Tree;

struct Trie { 

    string Hash[7];     // string 배열 선언
    string layers[4][8]; 
    int a = 0;

    //*********************** sha256 ***********************
    string sha256(const string str)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);
        stringstream ss;

        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss << hex << setw(2) << setfill('0') << (int)hash[i];
        }
        return ss.str();
    }

    //*********************** 첫 주소값을 해시화 ***********************
    string *primtive_hash(int N, string value[]){     // N --> main함수에서 줌

        cout << "layers : " << a << endl;

        // 맨 처음 각 값에 대한 hash를 취해줌
        for (int i = 0; i < N; i++){ 
            Hash[i] = sha256(value[i]);
            cout << "leaf node [" << i + 1 << "] : " << Hash[i] << endl;  // value 각각을 hash화한 hash 값 출력

            layers[0][i] = Hash[i];
        }

        return Hash;
    }

    //*********************** 트리 생성 ***********************
    string *Make_Tree(int N, string hashNode[]){
        for (int i = 0; i < a + 1; i++){
            cout << "[" << endl;
            for (int j = 0; j < 8; j++){
                cout << layers[i][j] << endl;
            }
            cout << "]" << endl;

        }
       
        string rootHash[1];     // 마지막 값 하나인 roothash 선언
        //int halfN = N / 2;  // 이진 트리 --> 층 올라가면 노드 개수 절반으로 줄어듦
        int halfN = (N - 1) / 2 + 1;
        
        a += 1;
        cout << "layers : " << a << endl;

        if (N == 2){    // 마지막 N == 1 일 때 rootHash 구하는 거

            rootHash[0] = sha256(hashNode[0]+hashNode[1]); 
            cout << "rootHash : " << rootHash[0] << endl;

            layers[a][0] = rootHash[0];

            // for (int i = 0; i < a+1; i++){
            //     cout << "[" << endl;
            //     for (int j = 0; j < pow(2, a); j++){
            //         cout << layers[i][j] <<endl;
            //     }
            //     cout << "]" << endl;
            // }

        }
        else{   // 각 층의 hash node 생성
            if(N % 2 != 0){     // node가 홀수개 일때 --> 짝수개로 맞춰줌
                hashNode[N] = hashNode[N - 1];    // 마지막 N+1번째에 N번째와 같은 노드 생성. ex) node가 9개면 마지막에 있는 9번째 node를 복제하여 10번째 임의의 node를 만들어서 총 짝수개로 맞춤
                layers[a - 1][N] = hashNode[N];
            }

            for (int i = 0; i < halfN; i++){    // node를 2쌍씩 묶어 합친 후 hash값으로 변환. N이 홀수인 경우를 대비해 (N - 1) / 2 + 1 라는 범위를 줌
                hashNode[i] = sha256(hashNode[2 * i] + hashNode[2 * i + 1]);    // 다음 층의 hashNode에 hashNode[0]+hashNode[1], hashNode[2]+hashNode[3], ...  값을 넣어줌 --> node 개수 절반으로 줄어듦
                layers[a][i] = hashNode[i];
                
                cout << "hashnode [" << i + 1 << "] : " << hashNode[i] << endl;    // 각 층의 hash 값 출력
            }

            for (int i = halfN; i < 6; i++){    // hashNode[i] 개수가 절반으로 줄어듦 --> hashNode의 [N / 2]번째부터 값을 초기화시켜줌
                hashNode[i] = {NULL}; 
            }
            
            Make_Tree(halfN, hashNode);   //재귀호출

        }

        return (string*) layers;     //rootHash 값 리턴

    }

    //*********************** 검증 ***********************
    string Valid(int num, string add){   // num : 몇번쨰인지, pt : 주소 값
   
        string v_layer[1];        

        v_layer[0] = sha256(add);   // 검증할 add를 hash화
        cout << "layer [0] : " << v_layer[0] << endl;

        int b = 0;
        while (b < 3){
            
            if (num % 2 == 0){      // [num]배열이 짝수번째인 경우 --> num + 1 과 연산
                v_layer[0] = sha256(v_layer[0] + layers[b][num + 1]);
            }
            else{       // [num]배열이 홀수번째인 경우 --> num - 1 과 연산
                v_layer[0] = sha256(layers[b][num - 1] + v_layer[0]);
            }

            cout << "layer [" << b + 1 << "] : " << v_layer[0] << endl;

            num = num / 2;  //
            b += 1;
        }

        if (v_layer[0] == layers[b][0]){
            cout << "True : " << v_layer[0] << endl;
        }
        else{
            cout << "False : " << v_layer[0] << endl;
        }

        return v_layer[0];

    }


};

 
int main(void){

    string value[7] = {"1234567890_1", "1234567890_2", "1234567890_3", "1234567890_4", 
                        "1234567890_5", "1234567890_6", "1234567890_7"};

    int N = sizeof(value)/sizeof(string);
    cout << "number of address : " << N << endl;    // 계정 개수

    Trie trie;
    string *Hash_First = trie.primtive_hash(N, value);

    string H[N+1];
    for (int i = 0; i < N; ++i) {
        H[i] = Hash_First[i];
    }
    
    trie.Make_Tree(N, H);

    trie.Valid(6, "1234567890_7");  //6+1번째
}



