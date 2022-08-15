#pragma once
#include "Word.h"
#include "converter.h"

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <set>

using namespace std;

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)> class Dictionary;
template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
class TrieNode {
private:
    Word* data;
    TrieNode* nxt[MAX_SIZE];
    friend class Dictionary<MAX_SIZE, getid, getchar>;
public:
    TrieNode();
    bool isLeaf() const;
    Word* get_data() const;
};

// this version of Trie contains characters on it edges
template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
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
    TrieNode<MAX_SIZE, getid, getchar>* pRoot;

    void remove_from_searchHistory(Word*);
    // the erasion of a node should set its pointer to nullptr for the sake of searchHistory and favoriteList
    bool internal_erase(const string&, TrieNode<MAX_SIZE, getid, getchar>*&, int);
    // the erasion of a node should set its pointer to nullptr for the sake of searchHistory and favoriteList
    bool internal_erase(const Word&, TrieNode<MAX_SIZE, getid, getchar>*&, int);
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
    Word* insert(const Word&);
    // insert a new word (only the word) to the dictionary
    Word* insert(const string&);
    // insert a new word (word + def) to the dictionary
    Word* insert(const string&, const string&);
    // find a node in the dictionary containing the word w
    Word* find(const Word&);
    Word* query(const Word&); // the same as find but don't save to searchHistory
    // find a node in the dictionary containing the string word
    Word* find(const string&);
    Word* query(const string&); // the same as find but don't save to searchHistory
    // this erasing feature requires that when a word is removed from the Trie, all the nodes above the removed nodes that 
    // lead nowhere should be deleted.
    // erase the node in the dictionary containing the struct w
    bool erase(const Word&);
    // erase the node in the dictionary containing the string w
    bool erase(const string&);
    
    // If there is a series of words which have at least 1 prefix in common, this algorithm chooses the longest one
    const Word* random_word() const;

    void getPrefixMatch(TrieNode<MAX_SIZE, getid, getchar>*, vector<string>&, const int&);
    vector<string> prefixMatch(const string&, const int&);

    vector<pair<string, string>> randomQuiz();

    void saveSerialTrie(TrieNode<MAX_SIZE, getid, getchar>*, fstream&);

    void loadSerialTrie(TrieNode<MAX_SIZE, getid, getchar>*, fstream&);

    //save and load data by serialization
    void saveDataStructures(string);

    void loadDataStructures(string path);
};

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
TrieNode<MAX_SIZE, getid, getchar>::TrieNode() {
    data = nullptr;
    for (int i = 0; i < MAX_SIZE; ++i)
        nxt[i] = nullptr;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
bool TrieNode<MAX_SIZE, getid, getchar>::isLeaf() const {
    for (int i = 0; i < MAX_SIZE; ++i)
        if (nxt[i])
            return false;
    return true;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Word* TrieNode<MAX_SIZE, getid, getchar>::get_data() const {
    return data;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
bool Dictionary<MAX_SIZE, getid, getchar>::add_to_favoriteList(Word* wrd) {
    for (Word* p : favoriteList) {
        if (p == wrd) return false;
    }
    favoriteList.push_back(wrd);
    return true;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
bool Dictionary<MAX_SIZE, getid, getchar>::remove_from_favoriteList(Word* wrd) {
    for (int i = 0; i < int(favoriteList.size()); ++i) {
        if (favoriteList[i] == wrd) {
            favoriteList.erase(favoriteList.begin() + i);
            return true;
        }
    }
    return false;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
void Dictionary<MAX_SIZE, getid, getchar>::remove_from_searchHistory(Word* wrd) {
    for (int i = 0; i < int(searchHistory.size()); ++i) {
        if (searchHistory[i] == wrd) {
            searchHistory.erase(searchHistory.begin() + i);
            --i;
        }
    }
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
bool Dictionary<MAX_SIZE, getid, getchar>::internal_erase(const string& w, TrieNode<MAX_SIZE, getid, getchar>*& cur, int cur_idx) {
    assert(cur);
    if (cur_idx == int(w.size())) {
        if (cur->data == nullptr) return false;
        remove_from_searchHistory(cur->data);
        remove_from_favoriteList(cur->data);
        delete cur->data;
        cur->data = nullptr;
        if (cur->isLeaf()) {
            delete cur;
            cur = nullptr;
        }
        return true;
    }

    int id {getid(w[cur_idx])};
    if (id == -1 || cur->nxt[id] == nullptr) return false;
    bool res {internal_erase(w, cur->nxt[id], cur_idx+1)};
    if (cur->data == nullptr && cur->isLeaf() && cur != pRoot) {
        delete cur;
        cur = nullptr;
    }
    return res;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
bool Dictionary<MAX_SIZE, getid, getchar>::internal_erase(const Word& w, TrieNode<MAX_SIZE, getid, getchar>*& cur, int cur_idx) {
    assert(cur);
    if (cur_idx == int(w.word.size())) {
        if (cur->data == nullptr) return false;
        remove_from_searchHistory(cur->data);
        remove_from_favoriteList(cur->data);
        delete cur->data;
        cur->data = nullptr;
        if (cur->isLeaf()) {
            delete cur;
            cur = nullptr;
        }
        return true;
    }

    int id {getid(w.word[cur_idx])};
    if (id == -1 || cur->nxt[id] == nullptr) return false;
    bool res {internal_erase(w, cur->nxt[id], cur_idx+1)};
    if (cur->data == nullptr && cur->isLeaf() && cur != pRoot) {
        delete cur;
        cur = nullptr;
    }
    return res;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Dictionary<MAX_SIZE, getid, getchar>::Dictionary() {
    pRoot = new TrieNode<MAX_SIZE, getid, getchar>();
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Dictionary<MAX_SIZE, getid, getchar>::~Dictionary() {
    if (pRoot == nullptr) return;
    queue<TrieNode<MAX_SIZE, getid, getchar>*> nodes;
    nodes.push(pRoot);
    while (nodes.size()) {
        TrieNode<MAX_SIZE, getid, getchar>*& cur {nodes.front()};
        nodes.pop();
        if (cur->data) {
            delete cur->data;
            cur->data = nullptr;
        }
        for (int i = 0; i < MAX_SIZE; ++i)
            if (cur->nxt[i])
                nodes.push(cur->nxt[i]);
        delete cur;
        cur = nullptr;
    }
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Word* Dictionary<MAX_SIZE, getid, getchar>::insert(const Word& w) {
    if (w.word.empty()) return nullptr; // for the erasion's sake, inserting an empty word is not allowed!
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    assert(cur);
    for (const char c : w.word) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                cur->nxt[id] = new TrieNode<MAX_SIZE, getid, getchar>();
            cur = cur->nxt[id];
        }
    }
    if (cur->data == nullptr) {
        cur->data = new Word(w);
        return cur;
    }
    else return nullptr; // the word already exists
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Word* Dictionary<MAX_SIZE, getid, getchar>::insert(const string& w) {
    if (w.empty()) return nullptr; // for the erasion's sake, inserting an empty word is not allowed!
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    assert(cur);
    for (const char c : w) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                cur->nxt[id] = new TrieNode<MAX_SIZE, getid, getchar>();
            cur = cur->nxt[id];
        }
    }
    if (cur->data == nullptr) {
        cur->data = new Word(w);
        return cur;
    }
    else return nullptr; // the word already exists
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Word* Dictionary<MAX_SIZE, getid, getchar>::insert(const string& w, const string& def) {
    if (w.empty()) return nullptr; // for the erasion's sake, inserting an empty word is not allowed!
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    assert(cur);
    for (const char c : w) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                cur->nxt[id] = new TrieNode<MAX_SIZE, getid, getchar>();
            cur = cur->nxt[id];
        }
    }
    if (cur->data == nullptr) {
        cur->data = new Word;
        cur->data->word = w;
    }
    if (cur->data->contain(def) != -1)
        return nullptr; // this definition already exists in the node of word
    cur->data->data.push_back(make_pair(def, "")); // definition without examples
    return cur;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Word* Dictionary<MAX_SIZE, getid, getchar>::find(const Word& w) {
    if (w.word.empty()) return nullptr;
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    assert(cur);
    for (const char c : w.word) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                return nullptr;
            cur = cur->nxt[id];
        }
    }
    if (cur->data == nullptr) return nullptr;
    else {
        searchHistory.push_back(cur->data);
        return cur->data;
    }
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Word* Dictionary<MAX_SIZE, getid, getchar>::query(const Word& w) {
    if (w.word.empty()) return nullptr;
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    assert(cur);
    for (const char c : w.word) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                return nullptr;
            cur = cur->nxt[id];
        }
    }
    if (cur->data == nullptr) return nullptr;
    else {
        return cur->data;
    }
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Word* Dictionary<MAX_SIZE, getid, getchar>::find(const string& w) {
    if (w.empty()) return nullptr;
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    assert(cur);
    for (const char c : w) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                return nullptr;
            cur = cur->nxt[id];
        }
    }
    if (cur->data == nullptr) return nullptr;
    else {
        searchHistory.push_back(cur->data);
        return cur->data;
    }
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
Word* Dictionary<MAX_SIZE, getid, getchar>::query(const string& w) {
    if (w.empty()) return nullptr;
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    assert(cur);
    for (const char c : w) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                return nullptr;
            cur = cur->nxt[id];
        }
    }
    if (cur->data == nullptr) return nullptr;
    else {
        return cur->data;
    }
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
bool Dictionary<MAX_SIZE, getid, getchar>::erase(const Word& w) {
    if (w.word.empty()) return true;
    return internal_erase(w, pRoot, 0);
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
bool Dictionary<MAX_SIZE, getid, getchar>::erase(const string& w) {
    if (w.empty()) return true;
    return internal_erase(w, pRoot, 0);
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
const Word* Dictionary<MAX_SIZE, getid, getchar>::random_word() const {
    const TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    assert(cur);
    while (cur) {
        vector<TrieNode<MAX_SIZE, getid, getchar>*> opts; opts.reserve(MAX_SIZE);
        // we have to ensure that the random child we branch into must lead to an existing word.
        // This is why the delete operation above should be carried out with care!
        for (int i = 0; i < MAX_SIZE; ++i) {
            if (cur->nxt[i])
                opts.push_back(cur->nxt[i]);
        }
        if (opts.empty()) {
            assert(cur->data); // a leaf node has to contain a word
            return cur->data;
        }
        else {
            int id {rand() % int(opts.size())};
            cur = opts[id];
        }
    }
    // this case shouldn't be reached
    assert(false);
    return nullptr;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
void Dictionary<MAX_SIZE, getid, getchar>::getPrefixMatch(TrieNode<MAX_SIZE, getid, getchar>* cur, vector<string>& ans, const int& maxNumOfEntries) {
    if (ans.size() == maxNumOfEntries) return;
    if (cur == nullptr) return;

    if (cur->data != nullptr) {
        ans.push_back(cur->data->word);
    }

    // for each of next exist characters
    for (int i = 0; i < MAX_SIZE; i++) {
        if (cur->nxt[i] != nullptr)
            getPrefixMatch(cur->nxt[i], ans, maxNumOfEntries);
    }
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
vector<string> Dictionary<MAX_SIZE, getid, getchar>::prefixMatch(const string& word, const int& maxNumOfEntries) {
    vector<string> ans;
    if (word.empty()) return {};
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    assert(cur);

    // traverse cur to end of word
    for (const char c : word) {
        int id {getid(c)};
        if (id == -1) return {};
        else {
            if (cur->nxt[id] == nullptr)
                return {};
            cur = cur->nxt[id];
        }
    }

    getPrefixMatch(cur, ans, maxNumOfEntries);
    return ans;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
vector<pair<string, string>> Dictionary<MAX_SIZE, getid, getchar>::randomQuiz() {
    vector<pair<string, string>> ans;
    for (int i = 0; i < 4; i++) {
        Word* randWord = random_word();
        string def = randWord->getFirstDef();

        ans.push_back({randWord->word, def});
    }

    return ans;
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
void Dictionary<MAX_SIZE, getid, getchar>::saveSerialTrie(TrieNode<MAX_SIZE, getid, getchar>* root, fstream& fout) {
    // end of a word
    if (root->data != nullptr) {
        fout << "]";
        root->data->saveToFile(fout);
    }
    // for each of next exist characters
    for (int i = 0; i < MAX_SIZE; i++) {
        if (root->nxt[i] != nullptr) {
            // write the character
            fout << getchar(i);
            saveSerialTrie(root->nxt[i], fout);
        }
    }
    // end of a character
    fout << ">";
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
void Dictionary<MAX_SIZE, getid, getchar>::loadSerialTrie(TrieNode<MAX_SIZE, getid, getchar>* root, fstream& fin) {
    char c;
    while (fin >> c) {
        // if c is '>', we have reached the end of the serialized Trie
        // so we backtrack to the parent node
        if (c == '>') break;
        // if c is ']', we have reached the end of a word
        if (c == ']') {
            Word* w = new Word();
            w->loadFromFile(fin);
            root->data = w;
            continue;
        }
        int id{getid(c)};
        // if c is a valid character, we create a new node and branch to it
        if (root->nxt[id] == nullptr)
            root->nxt[id] = new TrieNode<MAX_SIZE, getid, getchar>();
        loadSerialTrie(root->nxt[id], fin);
    }
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
void Dictionary<MAX_SIZE, getid, getchar>::saveDataStructures(string path) {
    fstream fout(path, ios::out);
    if (!fout.is_open()) {
        cerr << "Error: cannot open file " << path << endl;
        return;
    }
    cerr << "\nSaving file\n";
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    saveSerialTrie(cur, fout);
    cerr << "\nDONE!\n";

    fout.close();
}

template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
void Dictionary<MAX_SIZE, getid, getchar>::loadDataStructures(string path) {
    fstream fin(path, ios::in);
    if (!fin.is_open()) {
        cerr << "Error: cannot open file " << path << endl;
        return;
    }
    TrieNode<MAX_SIZE, getid, getchar>* cur {pRoot};
    loadSerialTrie(cur, fin);

    fin.close();
}
