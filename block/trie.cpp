#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>
#include <openssl/evp.h>


using namespace std;
typedef unsigned char BYTE; // 1 byte (8 bit) 0~255, BYTE는 char형의 배열
string strResult; //


struct Trie {
    bool finish; // 끝나는 지점을 표시해줌
    Trie* node[26]; // 26가지 알파벳에 대한 트라이

    Trie() {    //노드를 할당한다
        finish = false;  
        for (int i = 0; i < 26; i++) 
            node[i] = NULL;
    }

    // 생성자
    /*Trie() : finish(false) {
        memset(node, 0, sizeof(node));
    }
    // 소멸자
    ~Trie() {
        for (int i = 0; i < 26; i++)
            if (node[i])
                delete node[i];
    }*/

    // 트라이에 'Str'이라는 문자열을 삽입
    void insert(const char* str) {
        if (*str == '\0')
            finish = true;  // 문자열이 끝나는 지점일 경우 표시
        else {
            int curr = *str - 'A';      //문자열에서 '현재문자'를 가져오는 과정이다. Str = "ABC" 일 때, *Str = 'A'가 된다.
            if (node[curr] == NULL)     //현재 연결된 노드가 없는 경우
                node[curr] = new Trie();    // 해당 노드를 생성해주는 과정, 탐색이 처음되는 지점일 경우 동적할당
            node[curr]->insert(str + 1);    //(재귀호출) 다음 문자 삽입, 해당 노드를 통해서 그 다음 문자열로 넘어가는 과정이다.
        }
    }

    //*******************재귀 호출 실행 과정*************************
    //node[curr]->insert(str + 1); Str = "ABC"라고 했을 경우, Str + 1을 하게 되면, "BC"가 호출되어진다.
    //if (node[curr] == NULL) node[curr] = new Trie(); 'A' 에서 'B'로 가는 노드가 존재하는지 확인하고, 없으므로 노드를 생성해준다.
    //node[curr]->insert(str + 1);'B'노드를 통해서 'C'로 재귀호출 !
    //if (node[curr] == NULL) node[curr] = new Trie(); 'C'노드가 없으므로 노드를 생성해주고, 그 다음 문자열로 넘어간다.
    //그런데, 그 다음 문자열은 문자열의 끝을 나타내는 '\0' 즉, null 이다.
    //따라서, 이 때 if (*str == '\0') finish = true; 에 걸리게 된다.


    // 트라이에서 문자열 찾기
    // 해당 문자열을 찾았다면 true를 반환, 그게 아니라면 false를 반환하도록 만든 코드
    bool find(const char* str) {
        if (*str == '\0') {     //찾고자 하는 문자열의 마지막까지 탐색을 했을 경우
            if (finish == true) //'C'는 Finish가 true로 설정되어 있음. 문자열의 마지막이라는 말.
                return true;
            return false;       // 문자열이 끝나는 위치를 반환
        } 

        int curr = *str - 'A';

        if (node[curr] == NULL) 
            return false;       // 찾는 값이 존재하지 않음, 노드가 생성되지 않았다면 바로 false를 반환해버리는 부분이다.

        return node[curr]->find(str + 1);   // 다음 문자를 탐색
    }



    void IsSorting(Trie *N, char str[], int Idx) {
        if (N->finish == true) 
            cout << str << endl;

        for (int i = 0; i < 26; i++){
            if (N->node[i] != NULL){
                char C = i + 'A';
                str[Idx] = C;
                N->IsSorting(N->node[i], str, Idx + 1);
            }
        }
    }

};




int main(void)
{
    Trie *Root = new Trie();
    const char *String[5];
    String[0] = "ZYX";
    String[1] = "BCG";
    String[2] = "ABC";
    String[3] = "BDE";
    String[4] = "ABCD";
 
    for (int i = 0; i < 5; i++){
        Root->insert(String[i]);
    }
 
    for (int i = 0; i < 26; i++){
        if (Root->node[i] != NULL){
            char str[5] = { NULL };
            str[0] = i + 'A';
            Root->IsSorting(Root->node[i], str, 1);
        }
    }
}