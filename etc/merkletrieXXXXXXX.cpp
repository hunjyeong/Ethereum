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

//vector<string> Tree;
string Tree;

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

string hashNode[7];     // 6개짜리 string 배열 선언
string Make_Tree(int N){     // N = 6 --> main함수에서 줌

    string value[7] = {"1234567890_1", "1234567890_2", "1234567890_3", "1234567890_4", "1234567890_5", "1234567890_6", "1234567890_7"};
    //string hashNode[N];
    string rootHash[1];     // 마지막 값 하나인 roothash 선언
    int halfN = N / 2;  // 이진 트리 --> 층 올라가면 노드 개수 절반으로 줄어듦

    if (N == 7){    // 맨 처음 각 값에 대한 hash를 취해줌
        for (int i = 0; i < N; i++){
            hashNode[i] = sha256(value[i]);
            cout << hashNode[i] << endl;  // value 각각을 hash화한 hash 값 출력
        }
    }

    if (N == 1){    // 마지막 N == 1 일 때 rootHash 구하는 거
        rootHash[0] = sha256(hashNode[0]+hashNode[1]); 
        //cout << "rootHash : " << rootHash[0] << endl;
    }
    else{   // 각 층의 hash node 생성
        if(N % 2 != 0){     // node가 홀수개 일때 --> 짝수개로 맞춰줌
            hashNode[N] = hashNode[N - 1];    // 마지막 N+1번째에 N번째와 같은 노드 생성. ex) node가 9개면 마지막에 있는 9번째 node를 복제하여 10번째 임의의 node를 만들어서 총 짝수개로 맞춤
        }

        for (int i = 0; i < (N - 1) / 2 + 1; i++){    // node를 2쌍씩 묶어 합친 후 hash값으로 변환. N이 홀수인 경우를 대비해 (N - 1) / 2 + 1 라는 범위를 줌
            hashNode[i] = sha256(hashNode[2 * i] + hashNode[2 * i + 1]);    // 다음 층의 hashNode에 hashNode[0]+hashNode[1], hashNode[2]+hashNode[3], ...  값을 넣어줌 --> node 개수 절반으로 줄어듦
            //cout << "각 층의 " << i << "번째 hashnode : " << hashNode[i] << endl;    // 각 층의 hash 값 출력
        }

        for (int i = halfN; i < 6; i++){    // hashNode[i] 개수가 절반으로 줄어듦 --> hashNode의 [N / 2]번째부터 값을 초기화시켜줌
            hashNode[i] = {NULL}; 
        }
        Make_Tree(halfN);   //재귀호출

    }

    return rootHash[0];     //rootHash 값 리턴
}


 
int main(void){

    int N = 7; //직접 설정
    Make_Tree(N);

}
