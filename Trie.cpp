#include "Trie.h"

TrieNode::TrieNode() //создаем дерево
{
    end_of_word = false;
    for (int i = 0; i < 26; i++) {
        children[i] = NULL;
    }
    letter = '\0';
}

void Trie::Insert(string str) // заполняем дерево
{
    TrieNode* current = &root;
    for (size_t i = 0; i < str.size(); i++) {
        if (current->children[str.at(i) - 'a'] == NULL) {
            current->children[str.at(i) - 'a'] = new TrieNode;
            current->children[str.at(i) - 'a']->letter = str.at(i);
        }
        current = current->children[str.at(i) - 'a'];
    }
    current->end_of_word = true;
}

TrieNode* Trie::Search(string str) // поиск префикса в словаре
{
    TrieNode* current = &root;

    for (int i = 0; i < str.size(); i++) {
        int ind = str.at(i) - 'a';
        if (current->children[ind]) {
            current = current->children[ind];
        }
        else {
            current = NULL;
            break;
        }
    }
    return current;
}

void Trie::search_Words(TrieNode* current, string prefix, string suffix, vector<string>& words) // формирование вектора доступных слов для префикса
{
    if (current->end_of_word and suffix.size() != 0) {
        words.push_back(prefix + suffix);
    }
    for (int i = 0; i < 26; i++) {
        string temp = suffix;
        if (current->children[i]) {
            temp += current->children[i]->letter;
            search_Words(current->children[i], prefix, temp, words);
        }
    }
}
