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

struct TRIE
{
    bool Finish;        // 문자열이 끝난 지점 표시 (Finish = True)
    TRIE *Node[26];     // Trie의 노드 --> 알파벳 26개
    TRIE(){ 
        Finish = false;  
        for (int i = 0; i < 26; i++) {
            Node[i] = NULL;
        }
    }

    //*********************** 노드 생성 ***********************
    void Insert(char *Str){

        if (*Str == NULL){      // 문자열이 끝나면 Finish = true로 설정
            Finish = true;
            return;
        }
    
        int Cur = *Str - 'A';       // 현재 문자를 가져오는 과정 Str = "ABC"일 때 *Str = 'A'가 됨
        
        if (Node[Cur] == NULL){         // 현재 연결된 노드가 없는 경우, 해당 노드 생성
            Node[Cur] = new TRIE();
        }
        
        Node[Cur]->Insert(Str + 1);     // 해당 노드를 통해서 그 다음 문자열로 넘어가는 과정 // Str = "ABC"인 경우, Str + 1 --> "BC"가 호출됨

        // "BC" 재귀 호출 --> Str = "BC"인 상태로 들어오고, 현재 노드는 "A"이다
        // A -> B 로 가는 노드가 존재하는지 확인 --> 없으면 new TRIE()로 노드 생성 
        // "B" 노드를 통해서 "C" 노드 재귀 호출
        // B -> C 로 가는 노드가 존재하는지 확인 --> 없으면 노드 생성
        // 그 다음 문자열이 없으면 "\0 = NULL"이므로 if (*Str == NULL){} 문에 걸리게 됨

    }


    //*********************** 특정 문자열 찾기 ***********************
    bool Find(char *Str){    
        
        if (*Str == NULL){   // 찾고자 하는 문자열을 마지막까지 탐색한 경우
        
            if (Finish == true){    // "ABC"를 찾는 경우 "C"까지 간 이후에 걸림 --> "C"의 finish가 true로 설정됨
                return true;
            }
            return false;
        }

        int Cur = *Str - 'A';       // 현재 문자를 가져오는 과정 Str = "ABC"일 때 *Str = 'A'가 됨

        if (Node[Cur] == NULL) {    // 노드가 생성되지 않았다면 false 반환
            return false;
        }

        return Node[Cur]->Find(Str + 1);    //재귀 호출
    }


    //*********************** 트리 정렬 ***********************
    void IsSorting(TRIE *N, char Str[], int Idx)   
    {
        if (N->Finish == true) {    // 마지막 문자열이면 출력
            cout << Str << endl;
        }

        for (int i = 0; i < 26; i++){   // 알파벳 개수만큼

            if (N->Node[i] != NULL){    //node가 비어있지 않으면 순서를 줌
                char C = i + 'A';       
                Str[Idx] = C;
                N->IsSorting(N->Node[i], Str, Idx + 1);
            } 
        }
    }
};

 
int main(void)
{
    TRIE *Root = new TRIE();
    char *String[5];
    String[0] = "ZYX";
    String[1] = "BCG";
    String[2] = "ABC";
    String[3] = "BDE";
    String[4] = "ABCD";

    for (int i = 0; i < 5; i++){
        Root->Insert(String[i]);
    }

    for (int i = 0; i < 26; i++){   // 정렬

        if (Root->Node[i] != NULL){
            char Str[5] = {NULL};       // string 5개
            Str[0] = i + 'A';           // 
            Root->IsSorting(Root->Node[i], Str, 1);
        }
    }

    cout << Root->Find(String[2]) <<endl;

}
