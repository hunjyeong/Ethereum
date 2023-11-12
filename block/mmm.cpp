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

using namespace std;

class TrieNode
{

public:
    TrieNode *children[17]; // 17-item

    bool end_of_addr; // 마지막 주소값
    char letter;      // 노드에 저장된 char형 16진수 문자

    TrieNode()
    {
        end_of_addr = false;
        for (int i = 0; i < 17; i++)
        {
            children[i] = NULL;
        }
        letter = '\0';
    }
};

class Trie
{

public:
    TrieNode root;
    char val[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'}; // branch node 역할을 할 16개 16진수 문자

    void Insert(string str)
    {

        TrieNode *current = &root;
        for (size_t i = 0; i < str.size(); i++)
        {

            if (current->children[str.at(i) - val[0]] == NULL)
            {                                                              // 해당 문자열이 없는 경우
                current->children[str.at(i) - val[0]] = new TrieNode;      // 새로운 trie node를 만듦
                current->children[str.at(i) - val[0]]->letter = str.at(i); // null 값이었던 node에 값 추가
            }

            current = current->children[str.at(i) - val[0]]; // 현재 위치 수정
        }
        current->end_of_addr = true;
    }

    TrieNode *Search(string str)
    {

        TrieNode *current = &root;
        for (size_t i = 0; i < str.size(); i++)
        {
            if (current->children[str.at(i) - val[0]])
            {                                                    // 현재 노드에 값이 담겨 있으면
                current = current->children[str.at(i) - val[0]]; // 다음 노드로 넘어가기위해 현재 값 넣음
            }
            else
            {
                current = NULL; // 값이 없으면 null
                break;
            }
        }
        return current;
    }

    void PrintLexical(TrieNode *current, string prefix, string suffix)
    {

        if (current->end_of_addr and suffix.size() != 0)
        {
            cout << prefix + suffix << endl;    // prefix가 같은 node를 구하고, 뒤에 값 붙여주는 부분
        }

        for (int i = 0; i < 17; i++)
        {
            string temp = suffix;
            if (current->children[i])
            {
                temp += current->children[i]->letter;
                PrintLexical(current->children[i], prefix, temp);
            }
        }
    }
};

int main()
{

    Trie T;

    T.Insert("646f");
    T.Insert("646f67");
    T.Insert("646f6765");
    T.Insert("686f727365");

    // Search for the prefix in the trie
    string prefix("64");    //64의 prefix를 가진 노드들 찾음

    TrieNode *current = T.Search(prefix);

    if (current == NULL or current == &T.root)
    {
        cout << "No words with matching prefix found" << endl;      // 찾는 prefix를 가진 노드가 없음
    }
    else
    {
        bool haschildren = false;
        for (int c = 0; c < 17; c++)
        {
            if (current->children[c] != NULL)
            {
                haschildren = true;
                break;
            }
        }
        // No words found with the prefix (only the prefix was found)
        if (haschildren == false)
        {
            cout << "No words with matching prefix found" << endl;
        }
        else
        {
            cout << "Word(s) with prefix: " << prefix << endl;
            T.PrintLexical(current, prefix, "");
        }
    }
    return 0;
}
