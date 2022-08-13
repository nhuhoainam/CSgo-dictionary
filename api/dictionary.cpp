#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>

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
    bool contain(const string& def) const {
        for (auto& p : data) {
            if (def == p.first)
                return true;
        }
        return false;
    }
    void show() const {
        std::cout << "The word is: " << word << std::endl;
        for (auto& p : data) {
            std::cout << "-Definition\n=Example+Explanation\n";
            std::cout << '-' << p.first << "\n=" << p.second << '\n';
        }
    }

    // get fist definition of that word
    string getFirstDef() {
        if (data.empty() || data[0].first.empty())
            return NULL;
        return data[0].first;
    }

    // File structure:
    // word
    // size of part_of_speech[0] part_of_speech[1] ... part_of_speech[9]
    // part_of_speech[i][0]
    // part_of_speech[i][1]
    // ...
    // void saveToFile(fstream &fout) {
    //     fout << word << "\n";
    //     for (int i = 0; i < 10; i++)
    //         fout << part_of_speech[i].size() << " ";
    //     fout << "\n";
    //     for (int i = 0; i < 10; i++) {
    //         for (int j = 0; j < part_of_speech[i].size(); j++)
    //             cout << part_of_speech[i][j] << "\n";
    //     }
    // }

    // void loadFromFile(fstream &fin) {
    //     getline(fin, word);
    //     int sizeOfSpeech[10];
    //     for (int i = 0; i < 10; i++)
    //         fin >> sizeOfSpeech[i];
    //     fin.get();

    //     for (int i = 0; i < 10; i++) {
    //         for (int j = 0; j < sizeOfSpeech[i]; j++)
    //             getline(fin, part_of_speech[i][j]);
    //     }
    // }
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
    const Word* get_data() const {
        return data;
    }
    // there might be overloads of set() depending on the demands of UI team, this one is just one of them
    // TrieNode<MAX_SIZE, getid>* set(const string& def, const string& new_def) {
    //     assert(idx < int(data->part_of_speech[pos].size()));
    //     data->part_of_speech[pos][idx] = new_def;
    //     return this;
    // }
    // fix this later
};

// this version of Trie contains characters on it edges
template <int MAX_SIZE, int (*getid)(char)>
class Dictionary {
    static_assert(MAX_SIZE > 0, "Size of the trie must be positive.");
private:
    TrieNode<MAX_SIZE, getid>* pRoot;

    void add_to_searchHistory(Word* node) {
        for (Word* p : searchHistory)
            if (p == node) return;
        searchHistory.push_back(node);
    }
    bool internal_erase(const string& w, TrieNode<MAX_SIZE, getid>*& cur, int cur_idx) {
        if (cur_idx == int(w.size())) {
            if (cur->data == nullptr) return false;
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
        if (cur->data == nullptr && cur->isLeaf()) {
            delete cur;
            cur = nullptr;
        }
        return res;
    }
    bool internal_erase(const Word& w, TrieNode<MAX_SIZE, getid>*& cur, int cur_idx) {
        if (cur_idx == int(w.word.size())) {
            if (cur->data == nullptr) return false;
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
        if (cur->data == nullptr && cur->isLeaf()) {
            delete cur;
            cur = nullptr;
        }
        return res;
    }
public:
    vector<Word*> searchHistory;
    vector<Word*> favoriteList;
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
        if (cur->data->contain(def))
            return nullptr; // this definition already exists in the node of word
        cur->data->data.push_back(make_pair(def, "")); // definition without examples
        return cur;
    }
    // find a node in the dictionary containing the word w
    TrieNode<MAX_SIZE, getid>* find(const Word& w) {
        if (w.word.empty()) return nullptr;
        TrieNode<MAX_SIZE, getid>* cur {pRoot};
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
            add_to_searchHistory(cur->data);
            return cur;
        }
    }
    // find a node in the dictionary containing the string word
    TrieNode<MAX_SIZE, getid>* find(const string& w) {
        if (w.empty()) return nullptr;
        TrieNode<MAX_SIZE, getid>* cur {pRoot};
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
            add_to_searchHistory(cur->data);
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

    // save and load data structures are not done yet!!!
    // void saveDataStructures(string path) {
    //     ofstream fout(path);
    //     TrieNode<MAX_SIZE, getid>* cur {pRoot};
    //     queue<TrieNode<MAX_SIZE, getid>*> nodeQueue;
    //     nodeQueue.push(cur);


    //     //BFS
    //     while (nodeQueue.size() != 0) {
    //         cur = nodeQueue.top();
    //         nodeQueue.pop();
    //         if (cur == nullptr) continue;
    //         for (int i = 0; i < MAX_SIZE; i++) {
    //             nodeQueue.push(cur[i]);
    //         }
    //     }
        
    //     fout.close();
    // }

    // void loadDataStructures(string path) {
    //     ifstream fin(path);
    //     TrieNode<MAX_SIZE, getid>* cur {pRoot};
    //     queue<TrieNode<MAX_SIZE, getid>*> nodeQueue;
    //     nodeQueue.push(cur);
        
    //     //BFS
    //     while (nodeQueue.size() != 0) {
    //         cur = nodeQueue.top();
    //         nodeQueue.pop();
    //         int numOfChild;
    //         fin >> numOfChild;
    //         for (int i = 0; i < numOfChild; i++) {
    //             // input a child character
    //             int child; cin >> child;
    //             // create child node and append that node to queue
    //             cur->nxt[child] = new TrieNode<MAX_SIZE, getid>();
    //             TrieNode<MAX_SIZE, getid> nChild = cur->nxt[child];
    //             nodeQueue.push(nChild);
    //         }
    //     }

    //     fin.close();
    // }
};

int getid(char c) {
    if ('a' <= c && c <= 'z') return c - 'a';
    else if ('A' <= c && c <= 'Z') return c - 'A';
    else if (c == '-') return 26;
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
    for (int i = 0; i < numWords; i++) {
        string word;
        getline(fin, word);
        word.erase(word.begin());
        vector<pair<string, string>> wordData;
        int numDefs;
        fin >> numDefs;
        fin.ignore();
        for (int j = 0; j < numDefs; j++) {
            string def;
            getline(fin, def);
            def.erase(def.begin());
            int numExamples;
            fin >> numExamples;
            fin.ignore();
            string examples = "";
            for (int k = 0; k < numExamples; k++) {
                string example;
                getline(fin, example);
                example.erase(example.begin());
                examples += example + " ";
            }
            wordData.push_back({def, examples});
        }
        cerr << word << endl;
        cerr << wordData.size() << endl;
        fileData.push_back({word, wordData});
    }

    fin.close();
}

//inserts data to dictionary
void insertData(Dictionary<27, getid> &dictionary, vector<pair<string, vector<pair<string, string>>>>& fileData) {
    cout << "Inserting data to dictionary..." << endl;
    for (auto wordData : fileData) {
        cerr << wordData.first << endl;
        Word word;
        word.word = wordData.first;
        word.data = wordData.second;
        word.show();
        dictionary.insert(word);
    }
}

int main()
{
    /*
    // testing the data structure
    Dictionary<27, getid> myDict;
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
    */

    // testing the data structure
    Dictionary<27, getid> myDict;
    vector<pair<string, vector<pair<string, string>>>> fileData;
    readFromFile("VieEng-no-accent-test.txt", fileData);
    insertData(myDict, fileData);
    // string w;
    // while (getline(cin, w)) {
    //     auto node {myDict.find(w)};
    //     if (node == nullptr) {
    //         cerr << "Word does not exist." << endl;
    //     }
    //     else {
    //         node->get_data()->show();
    //     }
    // }

    return 0;
}