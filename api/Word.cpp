#include "api/Word.h"

Word::Word(const string& w) {
    word = w;
}

Word::Word(const Word& other) {
    *this = other;
}

int Word::contain(const string& def) const {
    for (int i = 0; i < int(data.size()); ++i) {
        if (def == data[i].first)
            return i;
    }
    return -1;
}

bool Word::set_def(int idx, const string& new_def) {
    if (idx < int(data.size())) {
        data[idx].first = new_def;
        return true;
    }
    else return false;
}

void Word::show() const {
    std::cout << "The word is: " << word << std::endl;
    for (auto& p : data) {
        std::cout << '-' << p.first << "\n=" << p.second << '\n';
    }
}

string Word::getFirstDef() {
    if (data.empty() || data[0].first.empty())
        return NULL;
    return data[0].first;
}

void Word::saveToFile(fstream &fout) {
    fout << "\n";
    fout << word << "\n";
    fout << data.size() << "\n";
    for (auto& p : data) {
        fout << p.first << "\n";
        fout << p.second << "\n";
    }
}

void Word::loadFromFile(fstream &fin) {
    fin.ignore();
    if (fin.eof()){
        cerr << "Error: file is empty" << endl;
        return;
    }
    getline(fin, word);
    int size;
    fin >> size;
    fin.ignore();
    for (int i = 0; i < size; i++) {
        string def, ex;
        getline(fin, def);
        getline(fin, ex);
        data.push_back(make_pair(def, ex));
    }
}
