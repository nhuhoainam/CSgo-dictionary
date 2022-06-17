#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

// 9 parts of speech in English
enum PoS {
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

// this version of Trie contains characters on it edges
class Trie {
public:
    struct Word {
        // 9 potential parts of speech of a word, each one may contain several definitions
        string word;
        vector<string> part_of_speech[9]; // the order is as in the enum PoS

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
    };
    vector<const Trie* const> searchHistory;
    vector<const Trie* const> favoriteList;
private:
    static constexpr int MAX {28}; // 26 lowercase English letters, '-', and ' '
    Word* data;
    Trie* nxt[MAX]; // a -> z, '-', and ' '

    inline int getid(char c) const {
        // 0 -> 25: a -> z
        // 26: '-'
        // 27: ' '
        if ('a' <= c && c <= 'z') return c - 'a';
        else if ('A' <= c && c <= 'Z') return c - 'A';
        else if (c == '-') return 26;
        else if (c == ' ') return 27;
        else return -1;
    }
    bool isLeaf() const {
        for (int i = 0; i < MAX; ++i)
            if (nxt[i])
                return false;
        return true;
    }
    void insert_to_searchHistory(const Trie* node) {
        for (const Trie* const p : searchHistory)
            if (p == node) return;
        searchHistory.push_back(node);
    }
public:
    Trie() {
        data = nullptr;
        for (int i = 0; i < MAX; ++i)
            nxt[i] = nullptr;
    }
    ~Trie() {
        queue<Trie*> nodes;
        nodes.push(this);
        while (nodes.size()) {
            Trie*& cur {nodes.front()};
            nodes.pop();
            if (cur->data) delete cur->data;
            for (int i = 0; i < MAX; ++i)
                if (cur->nxt[i])
                    nodes.push(cur->nxt[i]);
            delete cur;
            cur = nullptr;
        }
    }
    Trie* insert(const string& word, const PoS& pos, const string& def) {
        Trie* cur {this};
        for (const char c : word) {
            int id {getid(c)};
            if (id == -1) return nullptr;
            else {
                if (cur->nxt[id] == nullptr)
                    cur->nxt[id] = new Trie();
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
    Trie* find(const string& word) {
        Trie* cur {this};
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
            insert_to_searchHistory(cur);
            return cur;
        }
    }
    bool erase(const string& word) {
        if (word.empty()) return true;
        Trie* cur {this};
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
        assert(false);
        return false;
    }
    Trie* set(const PoS& pos, int idx, const string& new_def) {
        // wait a sec, there are multiple definitions, so which definition do you want to edit?
        // -> should depend on the UI to give users the options of which definition is edited
        assert(idx < int(this->data->part_of_speech[pos].size()));
        this->data->part_of_speech[pos][idx] = new_def;
        return this;
    }
    const Word* get_data() const {
        return data;
    }
    // this feature requires that when a word is removed from the Trie, all the nodes above the removed nodes that lead nowhere
    // should be deleted.
    const Word* pick_random() const {
        const Trie* cur {this};
        while (true) {
            if (cur->data) return cur->data;
            vector<Trie*> opts; opts.reserve(MAX);
            for (int i = 0; i < MAX; ++i) {
                if (cur->nxt[i])
                    opts.push_back(cur->nxt[i]);
            }
            int id {rand() % int(opts.size())};
            cur = opts[id];
        }
        assert(false);
    }
};
constexpr int Trie::MAX;