/*

TESTED FEATURES:
- struct Word (except for Word::getFirstDef())
- class TrieNode
- Dictionary::insert()
- Dictionary::find()
- Dictionary::erase() as well as Dictionary::internal_erase()
- Dictionary::add_to_favoriteList()
- Dictionary::remove_from_favoriteList()
- Dictionary::remove_from_searchHistory()
- getid_EngEng()
- readFromFile()
- insertData()

*/

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <set>

using namespace std;

struct Word {
    string word;
    vector<pair<string, string>> data; // including definitions + their corresponding examples

    Word(const string& w) {
        word = w;
    }
    Word(const Word& other) {
        *this = other;
    }

    Word() {}
    // return the index of the definition in the vector "data", return -1 if the definition is not found
    int contain(const string& def) const {
        for (int i = 0; i < int(data.size()); ++i) {
            if (def == data[i].first)
                return i;
        }
        return -1;
    }

    // edit the definition of this word, if that definition exists.
    bool set_def(int idx, const string& new_def) {
        if (idx < int(data.size())) {
            data[idx].first = new_def;
            return true;
        }
        else return false;
    }

    void show() const {
        std::cout << "The word is: " << word << std::endl;
        for (auto& p : data) {
            std::cout << '-' << p.first << "\n=" << p.second << '\n';
        }
    }

    // get fist definition of that word
    string getFirstDef() {
        if (data.empty() || data[0].first.empty())
            return NULL;
        return data[0].first;
    }
    //not done yet
    // File structure:

    void saveToFile(fstream &fout) {
    }

    void loadFromFile(fstream &fin) {
    }
};

template <int MAX_SIZE, int (*getid)(char)> class Dictionary;

template <int MAX_SIZE, int (*getid)(char)>
class TrieNode {
private:
    Word* data;
    TrieNode* nxt[MAX_SIZE];
    friend class Dictionary<MAX_SIZE, getid>;
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
    Word* get_data() const {
        return data;
    }
};

// this version of Trie contains characters on it edges
template <int MAX_SIZE, int (*getid)(char)>
class Dictionary {
    static_assert(MAX_SIZE > 0, "Size of the trie must be positive.");
public:
    // note that there may be multiple entries in the search history based on the lastest search
    vector<Word*> searchHistory;
    vector<Word*> favoriteList;
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
private:
    TrieNode<MAX_SIZE, getid>* pRoot;

    void remove_from_searchHistory(Word* wrd) {
        for (int i = 0; i < int(searchHistory.size()); ++i) {
            if (searchHistory[i] == wrd) {
                searchHistory.erase(searchHistory.begin() + i);
                --i;
            }
        }
    }
    // the erasion of a node should set its pointer to nullptr for the sake of searchHistory and favoriteList
    bool internal_erase(const string& w, TrieNode<MAX_SIZE, getid>*& cur, int cur_idx) {
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
    // the erasion of a node should set its pointer to nullptr for the sake of searchHistory and favoriteList
    bool internal_erase(const Word& w, TrieNode<MAX_SIZE, getid>*& cur, int cur_idx) {
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
public:
    Dictionary() {
        pRoot = new TrieNode<MAX_SIZE, getid>();
    }
    ~Dictionary() {
        // saveToFile();

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

    // file structure:
    // 
    // void saveToFile(string path) {
    //     fstream fout(path);

    //     fin.close();
    // }
    // void loadFromFile(string path);

    // insert a new word (word + definitions + corresponding examples) to the dictionary
    TrieNode<MAX_SIZE, getid>* insert(const Word& w) {
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
    // insert a new word (only the word) to the dictionary
    TrieNode<MAX_SIZE, getid>* insert(const string& w) {
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
    // insert a new word (word + def) to the dictionary
    TrieNode<MAX_SIZE, getid>* insert(const string& w, const string& def) {
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
    // find a node in the dictionary containing the word w
    TrieNode<MAX_SIZE, getid>* find(const Word& w) {
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
    // find a node in the dictionary containing the string word
    TrieNode<MAX_SIZE, getid>* find(const string& w) {
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
    // this erasing feature requires that when a word is removed from the Trie, all the nodes above the removed nodes that 
    // lead nowhere should be deleted.
    // erase the node in the dictionary containing the struct w
    bool erase(const Word& w) {
        if (w.word.empty()) return true;
        return internal_erase(w, pRoot, 0);
    }
    // erase the node in the dictionary containing the string w
    bool erase(const string& w) {
        if (w.empty()) return true;
        return internal_erase(w, pRoot, 0);
    }
    
    // If there is a series of words which have at least 1 prefix in common, this algorithm chooses the longest one
    const Word* random_word() const {
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

    void getPrefixMatch(TrieNode<MAX_SIZE, getid>* cur, vector<string>& ans, const int& maxNumOfEntries) {
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
    vector<string> prefixMatch(const string& word, const int& maxNumOfEntries) {
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

    vector<pair<string, string>> randomQuiz() {
        vector<pair<string, string>> ans;
        for (int i = 0; i < 4; i++) {
            Word* randWord = random_word();
            string def = randWord->getFirstDef();

            ans.push_back({randWord->word, def});
        }

        return ans;
    }

    void saveSerialTrie(TrieNode<MAX_SIZE, getid>* pRoot, fstream& fout) {
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

    void loadSerialTrie(TrieNode<MAX_SIZE, getid>* pRoot, fstream& fin) {
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

    //save and load data by serialization
    void saveDataStructures(string path) {
        ofstream fout(path);
        TrieNode<MAX_SIZE, getid>* cur {pRoot};
        saveSerialTrie(cur, fout);
        
        fout.close();
    }

    void loadDataStructures(string path) {
        ifstream fin(path);
        TrieNode<MAX_SIZE, getid>* cur {pRoot};
        loadSerialTrie(cur, fin);

        fin.close();
    }
};

// The Eng-Eng dataset consists of 41 characters in total.
int getid_EngEng(char c) {
    
    // lowercase and uppercase letters are treated as the same
    if ('a' <= c && c <= 'z')
        return c - 'a';
    else if ('A' <= c && c <= 'Z')
        return c - 'A';
    else if ('0' <= c && c <= '9')
        return 26 + c - '0';
    else if (c == ' ') return 36;
    else if (c == '-') return 37;
    else if (c == '.') return 38;
    else if (c == '/') return 39;
    else if (c == '\'') return 40;
    else return -1;
}

int getidEmotion(char c) {
    if (c < 0 || c >= 256) return -1;
    return c;
}

void readFromFileSlang(string path, vector <string> &slang, vector <string> &meaning) {
    ifstream fin(path);
    string a, b, input;

    while(getline(fin, input))
    {
        a = input.substr(0, input.find("`"));
        b = input.substr(input.find("`") + 1, input.size()-1);

        slang.push_back(a);
        meaning.push_back(b);
    }

    fin.close();
}

void readFromFile(string path, vector<pair<string, vector<pair<string, string>>>>& fileData) {
    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "Cannot read file." << endl;
        return;
    }

    int numWords;
    fin >> numWords;
    fin.ignore();
    // fstream fout("dataread.txt", ios::out);
    // if (!fout.is_open())
    //     assert(false);
    for (int i = 0; i < numWords; i++) {
        string word;
        // avoid bad input data, namely, an empty line
        do getline(fin, word);
        while (word.empty());
        word.erase(word.begin());
        vector<pair<string, string>> wordData;
        int numDefs;
        fin >> numDefs;
        fin.ignore();
        for (int j = 0; j < numDefs; j++) {
            string def;
            do getline(fin, def);
            while (def.empty());
            def.erase(def.begin());
            int numExamples;
            fin >> numExamples;
            fin.ignore();
            string examples = "";
            for (int k = 0; k < numExamples; k++) {
                string example;
                do getline(fin, example);
                while (example.empty());
                example.erase(example.begin());
                examples += example + " ";
            }
            wordData.push_back({def, examples});
        }
        // cerr << word << endl;
        // cerr << wordData.size() << endl;
        fileData.push_back({word, wordData});
    }

    fin.close();
}

//inserts data to dictionary
template <int MAX_SIZE, int (*getid)(char)>
void insertData(Dictionary<MAX_SIZE, getid> &dictionary, vector<pair<string, vector<pair<string, string>>>>& fileData) {
    // cout << "Inserting data to dictionary..." << endl;
    for (auto wordData : fileData) {
        Word word;
        word.word = wordData.first;
        word.data = wordData.second;
        // word.show();
        dictionary.insert(word);
    }
}

int main()
{
    // testing the data structure

    vector<pair<string, vector<pair<string, string>>>> fileData;
    readFromFile("..\\dictionary-data\\EngEng.txt", fileData);
    Dictionary<41, getid_EngEng> myDict;
    insertData(myDict, fileData);
    // cout << "0. Insert a new definition.\n"
    //         "1. Edit an existing definition.\n"
    //         "2. Remove a word.\n"
    //         "3. Show the information of a word.\n"
    //         "4. Show the search history.\n"
    //         "5. Show the favorite list.\n"
    //         "6. Add a word to the favorite list.\n"
    //         "7. Remove a word from the favorite list.\n"
    //         "8. Exit.\n";
    // while (true) {
    //     int opt;
    //     cout << "\nYour option: ";
    //     (cin >> opt).get();
    //     if (opt == 0) {
    //         cout << "Enter the word: ";
    //         string w;
    //         getline(cin, w);
    //         cout << "Enter the word's definition: ";
    //         string d;
    //         getline(cin, d);
    //         auto res {myDict.insert(w, d)};
    //         if (res) {
    //             cout << "Definition is added successfully.\n";
    //         }
    //         else {
    //             cout << "Definition is not added successfully.\n";
    //         }
    //     }
    //     else if (opt == 1) {
    //         cout << "Enter the word: ";
    //         string w;
    //         getline(cin, w);
    //         auto node {myDict.find(w)};
    //         cout << "Enter the word's definition: ";
    //         string d;
    //         getline(cin, d);
    //         int idx {node->get_data()->contain(d)};
    //         if (idx == -1) {
    //             cout << "This definition does not exist.\n";
    //         }
    //         else {
    //             cout << "Enter new definition: ";
    //             string new_d;
    //             getline(cin, new_d);
    //             if (node->get_data()->set_def(idx, new_d))
    //                 cout << "The definition is edited successfully.\n";
    //             else cout << "The definition is not edited successfully.\n";
    //         }
    //     }
    //     else if (opt == 2) {
    //         cout << "Enter the word: ";
    //         string w;
    //         getline(cin, w);
    //         bool res {myDict.erase(w)};
    //         if (res) cout << "The removal is successful.\n";
    //         else cout << "The removal is unsuccessful.\n";
    //     }
    //     else if (opt == 3) {
    //         cout << "Enter the word: ";
    //         string w;
    //         getline(cin, w);
    //         auto res {myDict.find(w)};
    //         if (!res) cout << "The word does not exist.\n";
    //         else {
    //             res->get_data()->show();
    //         }
    //     }
    //     else if (opt == 4) {
    //         cout << "Your search history:\n";
    //         for (Word* w : myDict.searchHistory) {
    //             w->show();
    //         }
    //     }
    //     else if (opt == 5) {
    //         for (Word* w : myDict.favoriteList)
    //             w->show();
    //     }
    //     else if (opt == 6) {
    //         cout << "Enter the word: ";
    //         string w;
    //         getline(cin, w);
    //         auto res {myDict.find(w)};
    //         if (!res) cout << "The word does not exist.\n";
    //         else {
    //             if (myDict.add_to_favoriteList(res->get_data()))
    //                 cout << "The word is added to the favorite list successfully.\n";
    //             else cout << "The word is not added to the favorite list successfully.\n";
    //         }
    //     }
    //     else if (opt == 7) {
    //         cout << "Enter the word: ";
    //         string w;
    //         getline(cin, w);
    //         auto res {myDict.find(w)};
    //         if (!res) cout << "The word does not exist.\n";
    //         else {
    //             if (myDict.remove_from_favoriteList(res->get_data()))
    //                 cout << "The word is removed from the favorite list successfully.\n";
    //             else cout << "The word is not removed from the favorite list successfully.\n";
    //         }
    //     }
    //     else break;
    // }

    return 0;
}