#include<iostream>
#include <string>
#include<vector>
using namespace std;

class TrieNode {

public:
    TrieNode* children[26];
    bool end_of_word;
    char letter;
    TrieNode();
};

class Trie {

public:

    TrieNode root;
    void Insert(string str); 
    TrieNode* Search(string str);
    void search_Words(TrieNode* current, string prefix, string suffix, vector<string>& words);
};