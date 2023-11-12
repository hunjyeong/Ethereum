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

// struct Branch {

//     string branch_hex[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};
        
// };

struct Trie {

    bool Finish;        // 문자열이 끝난 지점 표시 (Finish = True)
    Trie *Node[8];     // Trie의 노드 --> 20bytes = 40개
    Trie(){ 
        Finish = false;  
        for (int i = 0; i < 8; i++) {
            Node[i] = NULL;
        }
    }

    string layers;
    string branch_hex[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};

    //char * current;
    //char str_node[8];
    char root_prefix_node[4][8];
    char layer_node[4][8][8];
    char leaf_node[4][8];
    int pre_a = 0;

    void insert (char *addr, int number){

        if (*addr == NULL){ // 문자열이 끝나면 Finish = true로 설정
            Finish = true;
            return;
        }

        cout << addr << endl;
        //cout << addr + 1 << endl;
        //cout << addr[0] << endl;
        //cout << addr[1] << endl;

        int size = sizeof(addr)/sizeof(char);
        //cout << size << endl; //8

        int exist = 0;

        for (int i = 0; i < size; i++){
            leaf_node[number][i] = addr[i];
        }

        if (number == 0){       //if (current == NULL) {
            for (int i = 0; i < size; i++){
                //leaf_node[0][i] = addr[i];
                root_prefix_node[0][i] = addr[i];
            }
            //layer_node = addr;
            //cout << current << endl;
        }
        else{
            //root_prefix_node에서 같은게 있는지 없는지 판단부터
            for (int a = 0; a < number + 1; a++){
                for (int b = 0; b < size; b++){
                    if (root_prefix_node[a][b] == addr[b]){
                        exist = 1;
                        break;
                    }
                }
            }

            if (exist == 0){
                pre_a += 1;

                for (int i = 0; i < size; i++){
                    root_prefix_node[pre_a][i] = addr[i];
                    //leaf_node[number][i] = addr[i];
                }

                cout << root_prefix_node[pre_a] << endl;
                cout << leaf_node[number] << endl;

            }
            else{
                for (int n = 0; n < number; n++){
                    for (int i = 0; i < size; i++){
                        if (root_prefix_node[n][i] != addr[i] && root_prefix_node[n][i] != NULL){

                            //cout << root_prefix_node[n] << endl;

                            //prefix_node update & layer_node update
                            for (int u = 0; u + i < size; u++){
                                layer_node[n][u] = root_prefix_node[n][i + u];

                                if (u + i == size - 1){
                                    for (int k = 0; i + k < size; k++){
                                        root_prefix_node[n][i + k] = NULL;
                                    }
                                }

                            }


                            // leaf_node에 update된 leaf node 값 입력
                            for (int j = 0; j + i < size; j++){
                
                                leaf_node[n][j] = leaf_node[n][i + j];
                                leaf_node[number][j] = addr[i + j];

                                //leaf_node 원래 있는 뒤에 값 비워주는 부분
                                if (j + i == size - 1){
                                    for (int k = 0; j + 1 + k < size; k++){
                                        leaf_node[n][j + 1 + k] = NULL;
                                    }
                                }

                            }

                            cout << root_prefix_node[n] << endl;
                            cout << leaf_node[n] << endl;
                            cout << leaf_node[number] << endl; 


                            // cout << sizeof(root_prefix_node[n])/sizeof(char) << endl;
                            // cout << sizeof(root_prefix_node)/sizeof(char) << endl;
                            break;
                        }
                    }
                }



            }

        }
    
    }

};



int main(void){
    int n = 4;
    char * String[n];
    String[0] = "ab4d2908";
    String[3] = "a7fe321e";
    String[1] = "ab419dba";
    String[2] = "ca375a24";

    Trie t;
    Trie *Root = new Trie();
    for (int i = 0; i < 3; i++){
        Root->insert(String[i], i);
    }





}