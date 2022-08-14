#pragma once
#include "Word.h"
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <set>

using namespace std;

template <int MAX_SIZE, int (*getid)(char)> class Dictionary;
template <int MAX_SIZE, int (*getid)(char)>
class TrieNode {
private:
    Word* data;
    TrieNode* nxt[MAX_SIZE];
    friend class Dictionary<MAX_SIZE, getid>;
public:
    TrieNode();
    bool isLeaf() const;
    Word* get_data() const;
};

// this version of Trie contains characters on it edges
template <int MAX_SIZE, int (*getid)(char)>
class Dictionary {
    static_assert(MAX_SIZE > 0, "Size of the trie must be positive.");
public:
    // note that there may be multiple entries in the search history based on the lastest search
    vector<Word*> searchHistory;
    vector<Word*> favoriteList;
    // to view the favorite list, the UI can operate on the vector favoriteList
    // functions with favorite list
    bool add_to_favoriteList(Word*);
    bool remove_from_favoriteList(Word*);
    
private:
    TrieNode<MAX_SIZE, getid>* pRoot;

    void remove_from_searchHistory(Word*);
    // the erasion of a node should set its pointer to nullptr for the sake of searchHistory and favoriteList
    bool internal_erase(const string&, TrieNode<MAX_SIZE, getid>*&, int);
    // the erasion of a node should set its pointer to nullptr for the sake of searchHistory and favoriteList
    bool internal_erase(const Word&, TrieNode<MAX_SIZE, getid>*&, int);
public:
    Dictionary();
    ~Dictionary();

    // file structure:
    // 
    // void saveToFile(string path) {
    //     fstream fout(path);

    //     fin.close();
    // }
    // void loadFromFile(string path);

    // insert a new word (word + definitions + corresponding examples) to the dictionary
    TrieNode<MAX_SIZE, getid>* insert(const Word&);
    // insert a new word (only the word) to the dictionary
    TrieNode<MAX_SIZE, getid>* insert(const string&);
    // insert a new word (word + def) to the dictionary
    TrieNode<MAX_SIZE, getid>* insert(const string&, const string&);
    // find a node in the dictionary containing the word w
    TrieNode<MAX_SIZE, getid>* find(const Word&);
    // find a node in the dictionary containing the string word
    TrieNode<MAX_SIZE, getid>* find(const string&);
    // this erasing feature requires that when a word is removed from the Trie, all the nodes above the removed nodes that 
    // lead nowhere should be deleted.
    // erase the node in the dictionary containing the struct w
    bool erase(const Word&);
    // erase the node in the dictionary containing the string w
    bool erase(const string&);
    
    // If there is a series of words which have at least 1 prefix in common, this algorithm chooses the longest one
    const Word* random_word() const;

    void getPrefixMatch(TrieNode<MAX_SIZE, getid>*, vector<string>&, const int&);
    vector<string> prefixMatch(const string&, const int&);

    vector<pair<string, string>> randomQuiz();

    void saveSerialTrie(TrieNode<MAX_SIZE, getid>*, fstream&);

    void loadSerialTrie(TrieNode<MAX_SIZE, getid>*, fstream&);

    //save and load data by serialization
    void saveDataStructures(string);

    void loadDataStructures(string path);
};

#include "Dictionary.cpp"