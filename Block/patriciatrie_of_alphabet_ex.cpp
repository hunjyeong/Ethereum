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

class TrieNode {

    public:
        TrieNode * children[26];
        // A flag that marks if the word ends on this particular node.
        bool end_of_word;
        // Character stored in this node
        char letter;
        TrieNode() {
            end_of_word = false;
            for (int i = 0; i < 26; i++) {
                children[i] = NULL;
            }
            letter = '\0';
        }
};

class Trie {

    public:

    TrieNode root;

    // Insert the word in the trie.
    // Check each character in the string 
    // If none of the children of the current node contains the character, 
    // create a new child of the current node for storing the character. */
    void Insert (string str) {
        TrieNode * current = &root;
        for (size_t i = 0; i < str.size(); i++) {
            if (current->children[str.at(i)-'a'] == NULL) {
                current->children[str.at(i)-'a'] = new TrieNode;
                current->children[str.at(i)-'a']->letter = str.at(i);
            }
            current = current->children[str.at(i)-'a'];
        }
        current->end_of_word = true;
    }

    // Search the word in trie
    TrieNode * Search (string str) {
        TrieNode * current = &root;
        for (size_t i = 0; i < str.size(); i++) {
            if (current->children[str.at(i)-'a']) {
                current = current->children[str.at(i)-'a'];
             } else {
                current = NULL;
                break;
             }
        }
        return current;
    }

    // Print the words with the specified prefix in lexical order
    void PrintLexical (TrieNode * current, string prefix, string suffix) {
        if (current->end_of_word and suffix.size() != 0) {
            cout << prefix+suffix << endl;
        }
        for (int i=0; i<26; i++) {
            string temp = suffix;
            if (current->children[i]) {
                temp += current->children[i]->letter;
                PrintLexical(current->children[i], prefix, temp);
            }
        }
    }
};

int main() {

    Trie T;

    // Insert word(s) in the trie
    T.Insert("we");
    T.Insert("walk");
    T.Insert("want");
    T.Insert("wish");
    T.Insert("wit");
    T.Insert("am");
    T.Insert("yo");
    T.Insert("will");
    T.Insert("wee");
    T.Insert("war");
    T.Insert("warp");
    T.Insert("win");

    // Search for the prefix in the trie
    string prefix("wi");

    TrieNode * current = T.Search(prefix);

    if (current == NULL or current == &T.root) {
        cout << "No words with matching prefix found" << endl;
    } else {
        // Prefix has been found in the tree, look for children
        bool haschildren = false;
        for (int c = 0; c < 26; c++) {
            if (current->children[c] != NULL) {
                 haschildren = true; break;
            }
        }
        // No words found with the prefix (only the prefix was found)
        if (haschildren == false) {
            cout << "No words with matching prefix found" << endl;
        } else {
            cout << "Word(s) with prefix: " << prefix << endl;
            T.PrintLexical(current, prefix, "");
        }
    }
    return 0;
}
