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
};

template <int MAX_SIZE, int (*getid)(char)> class dictionary;

// this version of Trie contains characters on it edges
template <int MAX_SIZE, int (*getid)(char)>
class dictionary {
    static_assert(MAX_SIZE > 0, "Size of the trie must be positive.");
private:

public:
    dictionary() {
    }
    ~dictionary() {
    }

    // insert a new word (word + definitions + corresponding examples) to the dictionary
    bool insert(const Word& w) {
        return true; //true: success, false: failed
    }
    // insert a new word (only the word) to the dictionary
    bool insert(const string& w) {
        return true; //true: success, false: failed
    }
    // insert a new word (word + def) to the dictionary
    bool insert(const string& w, const string& def) {
        return true; //true: success, false: failed
    }
    // find a node in the dictionary containing the string word
    Word* find(const string& w) {
        Word* testData = new Word("keyword");
        testData->data.push_back(make_pair("definition", "example"));
        return testData;
    }
    // this erasing feature requires that when a word is removed from the Trie, all the nodes above the removed nodes that 
    // lead nowhere should be deleted.
    // erase the node in the dictionary containing the struct w
    bool erase(const Word& w) {
         return true; //true: success, false: failed
    }
    // erase the node in the dictionary containing the string w
    bool erase(const string& w) {
        return true; //true: success, false: failed
    }
    
    // get random word
    const Word* random_word() const {
        Word* testData = new Word("keyword");
        testData->data.push_back(make_pair("definition", "example"));
        return testData;
    }

    // functions with favorite list
    bool add_to_favoriteList(Word* wrd) {
        return true;
    }
    bool remove_from_favoriteList(Word* wrd) {
        return true;
    }

    vector<string> prefixMatch(const string& word, const int& maxNumOfEntries) {
        vector<string> ans{"word1, word2, word3, word4"};
        return ans;
    }

    vector<pair<string, string>> randomQuiz() {
        vector<pair<string, string>> ans;
        ans.push_back(make_pair("word1", "def1"));
        ans.push_back(make_pair("word2", "def2"));
        ans.push_back(make_pair("word3", "def3"));
        ans.push_back(make_pair("word4", "def4"));

        return ans;
    }
};

int main() {

    return 0;
}
