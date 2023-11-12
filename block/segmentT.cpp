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

vector<int> SegmentTree;


int Make_SegmentTree(int Node, int Start, int End){     
    //{ 현재 노드 번호 , 시작 범위 , 마지막 범위 } --> [ 1, 0 , N - 1 ] 로 첫 호출을 시작
    //1번 인덱스부터 사용할 것이기 때문에 가장 초기에 호출되는 현재노드번호 = 1 로 호출될 것이다.
    //시작범위는 배열의 시작범위이다.즉 '0'을 의미한다.
    //마지막범위는 배열의 마지막범위이다. 즉, 'N - 1'을 의미한다.

    int Arr[5] = { 1, 2, 3, 4, 5 };

    if (Start == End) 
        return SegmentTree[Node] = Arr[Start];
    
    int Mid = (Start + End) / 2;    //1. 주어진 범위를 반으로 나눈다.
    int Left_Result = Make_SegmentTree(Node * 2, Start, Mid);   //2. 나눠진 2개의 범위에 대해서 '왼쪽범위'에 대한 재귀호출을 한다. '왼쪽범위(노드번호 = 노드번호 x 2)'
    int Right_Result = Make_SegmentTree(Node * 2 + 1, Mid + 1, End);    //3. 나눠진 2개의 범위에 대해서 '오른쪽범위'에 대한 재귀호출을 한다. '오른쪽범위(노드번호 = 노드번호 x 2 + 1)'
                                                                        //4. 위의 과정을 반복한다. 이진트리 --> 항상 범위를 반으로 나누는 것이 중요
    SegmentTree[Node] = Left_Result + Right_Result;     // '왼쪽범위'와 '오른쪽범위' 를 탐색하면서 구한 값들을 저장하는 과정이다.
 
    return SegmentTree[Node];
}

int Sum(int Node, int Start, int End, int Left, int Right){
    //Node = 노드 번호
    //Start , End = 해당 노드가 포함하고 있는 범위
    //Left, Right = 우리가 구하고자 하는 범위

    if (Left > End || Right < Start) 
        return 0;
    if (Left <= Start && End <= Right) 
        return SegmentTree[Node];
 
    int Mid = (Start + End) / 2;
    int Left_Result = Sum(Node * 2, Start, Mid, Left, Right);
    int Right_Result = Sum(Node * 2 + 1, Mid + 1, End, Left, Right);
    return Left_Result + Right_Result;
}

 
int main(void){

    int N = 5; //직접 설정
    int Arr[5] = { 1, 2, 3, 4, 5 };

    //세그먼트 트리의 초기 크기를 설정하는 과정
    int Tree_Height = (int)ceil(log2(N));       //트리의 높이 = ceil(log2(N))
    int Tree_Size = (1 << (Tree_Height + 1));   //세그먼트 트리의 크기 = (1 << (트리의 높이 + 1) )
    SegmentTree.resize(Tree_Size);
    Make_SegmentTree(1, 0, N - 1);
    cout << Make_SegmentTree(1, 0, N - 1) << endl;
    int Index = 1;
    int Value = 5;
    int Diff = Value - Arr[Index];
    Sum(1, 0, N - 1, 0, 5);
    cout << Sum(1, 0, N - 1, 0, 3) << endl;
}