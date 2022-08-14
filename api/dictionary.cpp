#include "Dictionary.h"

template <int MAX_SIZE, int (*getid)(char)>
TrieNode<MAX_SIZE, getid>::TrieNode() {
    data = nullptr;
    for (int i = 0; i < MAX_SIZE; ++i)
        nxt[i] = nullptr;
}

template <int MAX_SIZE, int (*getid)(char)>
bool TrieNode<MAX_SIZE, getid>::isLeaf() const {
    for (int i = 0; i < MAX_SIZE; ++i)
        if (nxt[i])
            return false;
    return true;
}

template <int MAX_SIZE, int (*getid)(char)>
Word* TrieNode<MAX_SIZE, getid>::get_data() const {
    return data;
}

template <int MAX_SIZE, int (*getid)(char)>
bool Dictionary<MAX_SIZE, getid>::add_to_favoriteList(Word* wrd) {
    for (Word* p : favoriteList) {
        if (p == wrd) return false;
    }
    favoriteList.push_back(wrd);
    return true;
}

template <int MAX_SIZE, int (*getid)(char)>
bool Dictionary<MAX_SIZE, getid>::remove_from_favoriteList(Word* wrd) {
    for (int i = 0; i < int(favoriteList.size()); ++i) {
        if (favoriteList[i] == wrd) {
            favoriteList.erase(favoriteList.begin() + i);
            return true;
        }
    }
    return false;
}

template <int MAX_SIZE, int (*getid)(char)>
void Dictionary<MAX_SIZE, getid>::remove_from_searchHistory(Word* wrd) {
    for (int i = 0; i < int(searchHistory.size()); ++i) {
        if (searchHistory[i] == wrd) {
            searchHistory.erase(searchHistory.begin() + i);
            --i;
        }
    }
}

template <int MAX_SIZE, int (*getid)(char)>
bool Dictionary<MAX_SIZE, getid>::internal_erase(const string& w, TrieNode<MAX_SIZE, getid>*& cur, int cur_idx) {
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

template <int MAX_SIZE, int (*getid)(char)>
bool Dictionary<MAX_SIZE, getid>::internal_erase(const Word& w, TrieNode<MAX_SIZE, getid>*& cur, int cur_idx) {
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

template <int MAX_SIZE, int (*getid)(char)>
Dictionary<MAX_SIZE, getid>::Dictionary() {
    pRoot = new TrieNode<MAX_SIZE, getid>();
}

template <int MAX_SIZE, int (*getid)(char)>
Dictionary<MAX_SIZE, getid>::~Dictionary() {
    if (pRoot == nullptr) return;
    queue<TrieNode<MAX_SIZE, getid>*> nodes;
    nodes.push(pRoot);
    while (nodes.size()) {
        TrieNode<MAX_SIZE, getid>*& cur {nodes.front()};
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

template <int MAX_SIZE, int (*getid)(char)>
TrieNode<MAX_SIZE, getid>* Dictionary<MAX_SIZE, getid>::insert(const Word& w) {
    if (w.word.empty()) return nullptr; // for the erasion's sake, inserting an empty word is not allowed!
    TrieNode<MAX_SIZE, getid>* cur {pRoot};
    assert(cur);
    for (const char c : w.word) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                cur->nxt[id] = new TrieNode<MAX_SIZE, getid>();
            cur = cur->nxt[id];
        }
    }
    if (cur->data == nullptr) {
        cur->data = new Word(w);
        return cur;
    }
    else return nullptr; // the word already exists
}

template <int MAX_SIZE, int (*getid)(char)>
TrieNode<MAX_SIZE, getid>* Dictionary<MAX_SIZE, getid>::insert(const string& w) {
    if (w.empty()) return nullptr; // for the erasion's sake, inserting an empty word is not allowed!
    TrieNode<MAX_SIZE, getid>* cur {pRoot};
    assert(cur);
    for (const char c : w) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                cur->nxt[id] = new TrieNode<MAX_SIZE, getid>();
            cur = cur->nxt[id];
        }
    }
    if (cur->data == nullptr) {
        cur->data = new Word(w);
        return cur;
    }
    else return nullptr; // the word already exists
}

template <int MAX_SIZE, int (*getid)(char)>
TrieNode<MAX_SIZE, getid>* Dictionary<MAX_SIZE, getid>::insert(const string& w, const string& def) {
    if (w.empty()) return nullptr; // for the erasion's sake, inserting an empty word is not allowed!
    TrieNode<MAX_SIZE, getid>* cur {pRoot};
    assert(cur);
    for (const char c : w) {
        int id {getid(c)};
        if (id == -1) return nullptr;
        else {
            if (cur->nxt[id] == nullptr)
                cur->nxt[id] = new TrieNode<MAX_SIZE, getid>();
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

template <int MAX_SIZE, int (*getid)(char)>
TrieNode<MAX_SIZE, getid>* Dictionary<MAX_SIZE, getid>::find(const Word& w) {
    if (w.word.empty()) return nullptr;
    TrieNode<MAX_SIZE, getid>* cur {pRoot};
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
        return cur;
    }
}

template <int MAX_SIZE, int (*getid)(char)>
TrieNode<MAX_SIZE, getid>* Dictionary<MAX_SIZE, getid>::find(const string& w) {
    if (w.empty()) return nullptr;
    TrieNode<MAX_SIZE, getid>* cur {pRoot};
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
        return cur;
    }
}

template <int MAX_SIZE, int (*getid)(char)>
bool Dictionary<MAX_SIZE, getid>::erase(const Word& w) {
    if (w.word.empty()) return true;
    return internal_erase(w, pRoot, 0);
}

template <int MAX_SIZE, int (*getid)(char)>
bool Dictionary<MAX_SIZE, getid>::erase(const string& w) {
    if (w.empty()) return true;
    return internal_erase(w, pRoot, 0);
}

template <int MAX_SIZE, int (*getid)(char)>
const Word* Dictionary<MAX_SIZE, getid>::random_word() const {
    const TrieNode<MAX_SIZE, getid>* cur {pRoot};
    assert(cur);
    while (cur) {
        if (cur->data) return cur->data;
        vector<TrieNode<MAX_SIZE, getid>*> opts; opts.reserve(MAX_SIZE);
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

template <int MAX_SIZE, int (*getid)(char)>
void Dictionary<MAX_SIZE, getid>::getPrefixMatch(TrieNode<MAX_SIZE, getid>* cur, vector<string>& ans, const int& maxNumOfEntries) {
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

template <int MAX_SIZE, int (*getid)(char)>
vector<string> Dictionary<MAX_SIZE, getid>::prefixMatch(const string& word, const int& maxNumOfEntries) {
    vector<string> ans;
    if (word.empty()) return {};
    TrieNode<MAX_SIZE, getid>* cur {pRoot};
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

template <int MAX_SIZE, int (*getid)(char)>
vector<pair<string, string>> Dictionary<MAX_SIZE, getid>::randomQuiz() {
    vector<pair<string, string>> ans;
    for (int i = 0; i < 4; i++) {
        Word* randWord = random_word();
        string def = randWord->getFirstDef();

        ans.push_back({randWord->word, def});
    }

    return ans;
}

template <int MAX_SIZE, int (*getid)(char)>
void Dictionary<MAX_SIZE, getid>::saveSerialTrie(TrieNode<MAX_SIZE, getid>* pRoot, fstream& fout) {
    // end of a word
    if (pRoot->data != nullptr) {
        fout << "]";
        pRoot->data->saveToFile(fout);
    }
    // for each of next exist characters
    for (int i = 0; i < MAX_SIZE; i++) {
        if (pRoot->nxt[i] != nullptr) {
            // write the character
            fout << (char)i;
            saveSerialTrie(pRoot->nxt[i], fout);
        }
    }
    // end of a character
    fout << ">";
}

template <int MAX_SIZE, int (*getid)(char)>
void Dictionary<MAX_SIZE, getid>::loadSerialTrie(TrieNode<MAX_SIZE, getid>* pRoot, fstream& fin) {
    char c;
    while (fin >> c) {
        // if c is '>', we have reached the end of the serialized Trie
        // so we backtrack to the parent node
        if (c == '>') break;
        // if c is ']', we have reached the end of a word
        if (c == ']') {
            Word* w = new Word();
            w->loadFromFile(fin);
            pRoot->data = w;
        }
        // if c is a valid character, we create a new node and branch to it
        if (pRoot->nxt[c] == nullptr)
            pRoot->nxt[c] = new TrieNode<MAX_SIZE, getid>();
        loadSerialTrie(pRoot->nxt[c], fin);
    }
}

template <int MAX_SIZE, int (*getid)(char)>
void Dictionary<MAX_SIZE, getid>::saveDataStructures(string path) {
    ofstream fout(path);
    TrieNode<MAX_SIZE, getid>* cur {pRoot};
    saveSerialTrie(cur, fout);
    
    fout.close();
}

template <int MAX_SIZE, int (*getid)(char)>
void Dictionary<MAX_SIZE, getid>::loadDataStructures(string path) {
    ifstream fin(path);
    TrieNode<MAX_SIZE, getid>* cur {pRoot};
    loadSerialTrie(cur, fin);

    fin.close();
}