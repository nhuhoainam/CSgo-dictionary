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

#include "Dictionary.h"
#include "converter.h"

using namespace std;
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