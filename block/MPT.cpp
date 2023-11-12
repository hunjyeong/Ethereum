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
#include <openssl/evp.h>
#include <map>


using namespace std;

typedef unsigned char BYTE; // 1 byte (8 bit) 0~255, BYTE는 char형의 배열

class TrieNode {

    public:
        TrieNode * valueNode[17];      // value : [0,1,2, ... , 9, A, B, C, D, E, F, value] - 17 item
        
        bool end_of_addr;   // A flag that marks if the word ends on this particular node.
        char letter;        // 이 노드에 저장된 주소

        TrieNode() {
            end_of_addr = false;    // true : 1 / false : 0 - size : 1 byte
            for (int i = 0; i < 17; i++) {
                valueNode[i] = NULL;
            }
            letter = '\0';
        }
};


struct Trie { 

    TrieNode root;

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


    void Insert (string str) {
        TrieNode * current = &root;

        for (size_t i = 0; i < str.size(); i++) {

            if (current->valueNode[str.at(i)-'0'] == NULL) {

                current->valueNode[str.at(i)-'0'] = new TrieNode;
                current->valueNode[str.at(i)-'a']->letter = str.at(i);

            }
            current = current->valueNode[str.at(i)-'a'];
        }
        current->end_of_addr = true;
    }


    //*********************** 트리 생성 ***********************
    // [] key - value ] 형태
    // key : value의 hash 값으로 
    
    string update(string node, path, string value){
        if path == '':
            curnode = db.get(node) if node else [ NULL ] * 17
            newnode = curnode.copy()
            newnode[-1] = value
        else:
            curnode = db.get(node) if node else [ NULL ] * 17 
            newnode = curnode.copy()
            newindex = update(curnode[path[0]],path[1:],value)
            newnode[path[0]] = newindex
        db.put(sha256(newnode),newnode)
        return sha256(newnode)
    }

};


/* data set
{
    "cab8" : "dog",
    "cabe" = "cat",
    "cabe" = "cat",
    "395" = "duck",
    "56f0" = "horse"
}
*/
int main(void){

    map<string, string> m;

    m["cab8"] = "dog";
    m["cabe"] = "cat";
    m["cabe"] = "cat";
    m["395"] = "duck";
    m["56f0"] = "horse";





    


}



