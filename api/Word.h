#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct Word {
    string word;
    vector<pair<string, string>> data; // including definitions + their corresponding examples
    bool isFavorite = false;

    Word() {}
    Word(const string&);
    Word(const Word&);
    // return the index of the definition in the vector "data", return -1 if the definition is not found
    int contain(const string&) const;
    // edit the definition of this word, if that definition exists.
    bool set_def(int, const string&);
    void show() const;
    // get fist definition of that word
    string getFirstDef();
    void saveToFile(fstream &);
    void loadFromFile(fstream &);
};
