#include "Dictionary.hpp"
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
template <int MAX_SIZE, int (*getid)(char), char (*getchar)(int)>
void insertData(Dictionary<MAX_SIZE, getid, getchar> &dictionary, vector<pair<string, vector<pair<string, string>>>>& fileData) {
    for (auto wordData : fileData) {
        Word word;
        word.word = wordData.first;
        word.data = wordData.second;
        // word.show();
        dictionary.insert(word);
    }
}
