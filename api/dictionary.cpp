#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

// 9 parts of speech in English
enum PoS {
    NONE, // for emojis and slangs
    NOUN,
    ADJECTIVE,
    ADVERB,
    VERB,
    CONJUNCTION,
    PRONOUN,
    PREPOSITION,
    DETERMINER,
    MODAL_VERB
};
PoS pos_val[10] {NONE, NOUN, ADJECTIVE, ADVERB, VERB, CONJUNCTION, PRONOUN, PREPOSITION, DETERMINER, MODAL_VERB};

struct Word {
    // 9 potential parts of speech of a word, each one may contain several definitions
    string word;
    vector<string> part_of_speech[10]; // the order is as in the enum PoS

    Word(const string& w) {
        word = w;
    }
    bool contain(const PoS& pos, const string& def) const {
        for (const string& s : part_of_speech[pos]) {
            if (s == def)
                return true;
        }
        return false;
    }
    void show() const {
        std::cout << "The word is: " << word << std::endl;
        for (int i = 0; i < 10; ++i) {
            std::cout << "Definitions in part of speech " << i << ":\n";
            for(const string& s : part_of_speech[i])
                std::cout << s << std::endl;
        }
    }
};

template <int MAX_SIZE, int (*getid)(char)> class dictionary;

template <int MAX_SIZE, int (*getid)(char)>
class TrieNode {
private:
    Word* data;
    TrieNode* nxt[MAX_SIZE];
    friend class dictionary<MAX_SIZE, getid>;
public:
    TrieNode() {
        data = nullptr;
        for (int i = 0; i < MAX_SIZE; ++i)
            nxt[i] = nullptr;
    }
    bool isLeaf() const {
        for (int i = 0; i < MAX_SIZE; ++i)
            if (nxt[i])
                return false;
        return true;
    }
    const Word* get_data() const {
        return data;
    }
    // there might be overloads of set() depending on the demands of UI team, this one is just one of them
    TrieNode<MAX_SIZE, getid>* set(const PoS& pos, int idx, const string& new_def) {
        assert(idx < int(data->part_of_speech[pos].size()));
        data->part_of_speech[pos][idx] = new_def;
        return this;
    }
};

// this version of Trie contains characters on it edges
template <int MAX_SIZE, int (*getid)(char)>
class dictionary {
    static_assert(MAX_SIZE > 0, "Size of the trie must be positive.");
private:
    TrieNode<MAX_SIZE, getid>* pRoot;

    void add_to_searchHistory(Word* node) {
        for (Word* p : searchHistory)
            if (p == node) return;
        searchHistory.push_back(node);
    }
public:
    // a pointer pointing to const to avoid changing the data, a const pointer to avoid pointing the pointer of the array
    // to somewhere else
    vector<Word*> searchHistory;
    vector<Word*> favoriteList;
    dictionary() {
        pRoot = new TrieNode<MAX_SIZE, getid>();
    }
    ~dictionary() {
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

    // main functions of dictionary
    TrieNode<MAX_SIZE, getid>* insert(const string& word, const PoS& pos, const string& def) {
        if (word.empty()) return nullptr; // for the erase feature's sake, inserting an empty word is not allowed
        TrieNode<MAX_SIZE, getid>* cur {pRoot};
        for (const char c : word) {
            int id {getid(c)};
            if (id == -1) return nullptr;
            else {
                if (cur->nxt[id] == nullptr)
                    cur->nxt[id] = new TrieNode<MAX_SIZE, getid>();
                cur = cur->nxt[id];
            }
        }
        if (cur->data == nullptr)
            cur->data = new Word(word);
        // check whether this definition has already existed
        if (cur->data->contain(pos, def)) return nullptr;
        // this definition does not exist, so insert it!
        cur->data->part_of_speech[pos].push_back(def);
        return cur;
    }
    TrieNode<MAX_SIZE, getid>* find(const string& word) {
        if (word.empty()) return nullptr;
        TrieNode<MAX_SIZE, getid>* cur {pRoot};
        for (const char c : word) {
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
            add_to_searchHistory(cur->data);
            return cur;
        }
    }
    bool erase(const string& word) {
        if (word.empty()) return true;
        TrieNode<MAX_SIZE, getid>* cur {pRoot};
        for (int i = 0; i < int(word.size()); ++i) {
            int id {getid(word[i])};
            if (id == -1) return false;
            else {
                if (cur->nxt[id] == nullptr)
                    return false;
                if (i == int(word.size()) - 1) {
                    if (cur->nxt[id]->data == nullptr) return false;
                    delete cur->nxt[id]->data;
                    cur->nxt[id]->data = nullptr;
                    if (cur->nxt[id]->isLeaf()) {
                        delete cur->nxt[id];
                        cur->nxt[id] = nullptr;
                    }
                    return true;
                }
                else cur = cur->nxt[id];
            }
        }
        // this case shouldn't be reached
        assert(false);
        return false;
    }
    // this feature requires that when a word is removed from the Trie, all the nodes above the removed nodes that lead nowhere
    // should be deleted.
    const Word* random_word() const {
        const TrieNode<MAX_SIZE, getid>* cur {this};
        while (true) {
            if (cur->data) return cur->data;
            vector<TrieNode<MAX_SIZE, getid>*> opts; opts.reserve(MAX_SIZE);
            // we have to ensure that the random child we branch into must lead to an existing word.
            // This is why the delete operation should be carried out with care!
            for (int i = 0; i < MAX_SIZE; ++i) {
                if (cur->nxt[i])
                    opts.push_back(cur->nxt[i]);
            }
            int id {rand() % int(opts.size())};
            cur = opts[id];
        }
        // this case shouldn't be reached
        assert(false);
        return nullptr;
    }

    // functions with favorite list
    bool add_to_favoriteList(Word* wrd) {
        for (Word* p : favoriteList) {
            if (p == wrd) return false;
        }
        favoriteList.push_back(wrd);
        return true;
    }
    bool remove_from_favoriteList(Word* wrd) {
        for (int i = 0; i < int(favoriteList.size()); ++i) {
            if (favoriteList[i] == wrd) {
                favoriteList.erase(favoriteList.begin() + i);
                return true;
            }
        }
        return false;
    }
    // to view the favorite list, the UI can operate on the vector favoriteList
};

int getid(char c)
{
    if ('a' <= c && c <= 'z') return c - 'a';
    else if ('A' <= c && c <= 'Z') return c - 'A';
    else if (c == '-') return 26;
    else return -1;
}

int main()
{
    // testing the data structure
    dictionary<27, getid> myDict;
    cout << "0. Insert a new definition.\n"
            "1. Edit an existing definition.\n"
            "2. Remove a word.\n"
            "3. Show the information of a word.\n"
            "4. Show the search history.\n"
            "5. Exit.\n";
    while (true) {
        int opt;
        cout << "\nYour option: ";
        cin >> opt;
        cin.get();
        if (opt == 0) {
            cout << "Enter the word: ";
            string w;
            getline(cin, w);
            int p;
            do {
                cout << "Enter part of speech (0 -> 9): ";
                cin >> p;
                if (p < 0 || p > 9)
                    cout << "Invalid part of speech. Please try again.\n";
            } while (p < 0 || p > 9);
            PoS pos {pos_val[p]};
            cout << "Enter the word's definition: ";
            string d;
            cin.get();
            getline(cin, d);
            auto res {myDict.insert(w, pos, d)};
            if (res) {
                cout << "Definition is added successfully.\n";
            }
            else {
                cout << "Definition is not added successfully.\n";
            }
        }
        else if (opt == 1) {
            cout << "Enter the word: ";
            string w;
            getline(cin, w);
            auto node {myDict.find(w)};
            int p;
            do {
                cout << "Enter part of speech (0 -> 9): ";
                cin >> p;
                if (p < 0 || p > 9)
                    cout << "Invalid part of speech. Please try again.\n";
            } while (p < 0 || p > 9);
            PoS pos {pos_val[p]};
            cout << "Enter the word's definition: ";
            string d;
            cin.get();
            getline(cin, d);
            if (!node->get_data()->contain(pos, d)) {
                cout << "This definition does not exist.\n";
            }
            else {
                cout << "Enter new definition: ";
                string new_d;
                getline(cin, new_d);
                for (int i = 0; i < int(node->get_data()->part_of_speech[pos].size()); ++i) {
                    if (node->get_data()->part_of_speech[pos][i] == d) {
                        node->set(pos, i, new_d);
                        break;
                    }
                }
                cout << "The definition is edited successfully.\n";
            }
        }
        else if (opt == 2) {
            cout << "Enter the word: ";
            string w;
            getline(cin, w);
            bool res {myDict.erase(w)};
            if (res) cout << "The removal is successful.\n";
            else cout << "The removal is unsuccessful.\n";
        }
        else if (opt == 3) {
            cout << "Enter the word: ";
            string w;
            getline(cin, w);
            auto res {myDict.find(w)};
            if (!res) cout << "The word does not exist.\n";
            else {
                res->get_data()->show();
            }
        }
        else if (opt == 4) {
            cout << "Your search history:\n";
            for (Word* w : myDict.searchHistory) {
                w->show();
            }
        }
        else break;
    }

    return 0;
}