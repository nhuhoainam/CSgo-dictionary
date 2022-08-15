#include "api/dictionary_collection.hpp"
#include "api/Utils.hpp"

const string EngEngSavedPath = "./dictionary-data/dictionary-eng-eng-saved-data.txt";
const string EngEngRawPath = "./dictionary-data/EngEng.txt";
const string EngEngHistoryPath = "./dictionary-data/dictionary-eng-eng-history-saved-data.txt";
const string EngEngFavoritePath = "./dictionary-data/dictionary-eng-eng-favorite-saved-data.txt";

const string EngVieSavedPath = "./dictionary-data/dictionary-eng-vie-saved-data.txt";
const string EngVieRawPath = "./dictionary-data/EngVie.txt";

const string VieEngSavedPath = "./dictionary-data/dictionary-vie-eng-saved-data.txt";
const string VieEngRawPath = "./dictionary-data/VieEng.txt";
const string VieEngFavoritePath = "./dictionary-data/dictionary-vie-eng-favorite-saved-data.txt";
const string VieEngHistoryPath = "./dictionary-data/dictionary-vie-eng-history-saved-data.txt";

const string EmoSavedPath = "./dictionary-data/dictionary-emoji-saved-data.txt";
const string EmoRawPath = "./dictionary-data/Emoji.txt";

bool fexist(const string &path) {
    ifstream f(path);
    return f.good();
}

DictCollection::DictCollection() {
    engEngDict = new EngEngDictionary;
    engVieDict = new EngVieDictionary;
    vieEngDict = new VieEngDictionary;
    emoDict = new EmojiDictionary;
}

void DictCollection::initRaw() {
    delete engEngDict;
    engEngDict = new EngEngDictionary;
    vector<pair<string, vector<pair<string, string>>>> fileData;
    readFromFile(EngEngRawPath, fileData);
    insertData(*engEngDict, fileData);
}

void DictCollection::init() {
    curDict = EngEng;

    // Eng - Eng Dictionary
    if (fexist(EngEngSavedPath)) {
        engEngDict->loadDataStructures(EngEngSavedPath);
    } else {
        vector<pair<string, vector<pair<string, string>>>> fileData;
        readFromFile(EngEngRawPath, fileData);
        insertData(*engEngDict, fileData);
    }
    if (fexist(EngEngHistoryPath)) {
        engEngDict->loadSearchHistoryList(EngEngHistoryPath);
    }
    if (fexist(EngEngFavoritePath)) {
        engEngDict->loadFavouristList(EngEngFavoritePath);
    }
    cerr << "Done";

    // Vie - Eng Dictionary
    if (fexist(VieEngSavedPath)) {
        vieEngDict->loadDataStructures(VieEngSavedPath);
    } else {
        vector<pair<string, vector<pair<string, string>>>> fileData;
        readFromFile(VieEngRawPath, fileData);
        insertData(*vieEngDict, fileData);
    }
    if (fexist(VieEngHistoryPath)) {
        vieEngDict->loadSearchHistoryList(VieEngHistoryPath);
    }
    if (fexist(VieEngFavoritePath)) {
        vieEngDict->loadFavouristList(VieEngFavoritePath);
    }
    cerr << "Done";
}

void DictCollection::close() {
    engEngDict->saveDataStructures(EngEngSavedPath);
    engEngDict->saveFavouristList(EngEngFavoritePath);
    engEngDict->saveSearchHistoryList(EngEngHistoryPath);

    vieEngDict->saveDataStructures(VieEngSavedPath);
    vieEngDict->saveFavouristList(VieEngFavoritePath);
    vieEngDict->saveSearchHistoryList(VieEngHistoryPath);
}

Word* DictCollection::find(const Word& w) {
    Word *rs;
    switch (curDict) {
    case EngEng:
        rs = engEngDict->find(w);
        break;
    case VieEng:
        rs = vieEngDict->find(w);
        break;
    case EngVie:
        rs = engVieDict->find(w);
        break;
    case Emoji:
        rs = emoDict->find(w);
        break;
    }
    return rs;
}

Word* DictCollection::query(const Word& w) {
    Word *rs;
    switch (curDict) {
    case EngEng:
        rs = engEngDict->query(w);
        break;
    case VieEng:
        rs = vieEngDict->query(w);
        break;
    case EngVie:
        rs = engVieDict->query(w);
        break;
    case Emoji:
        rs = emoDict->query(w);
        break;
    }
    return rs;
}

const Word* DictCollection::random_word() const {
    switch (curDict) {
    case EngEng:
        return engEngDict->random_word();
        break;
    case VieEng:
        return vieEngDict->random_word();
        break;
    case EngVie:
        return engVieDict->random_word();
        break;
    default:
        return emoDict->random_word();
        break;
    }
}

void DictCollection::add_to_favoriteList(Word *w) {
    switch (curDict) {
    case EngEng:
        engEngDict->add_to_favoriteList(w);
        break;
    case VieEng:
        vieEngDict->add_to_favoriteList(w);
        break;
    case EngVie:
        engVieDict->add_to_favoriteList(w);
        break;
    case Emoji:
        emoDict->add_to_favoriteList(w);
        break;
    }
}

void DictCollection::remove_from_favoriteList(Word *w) {
    switch (curDict) {
    case EngEng:
        engEngDict->remove_from_favoriteList(w);
        break;
    case VieEng:
        vieEngDict->remove_from_favoriteList(w);
        break;
    case EngVie:
        engVieDict->remove_from_favoriteList(w);
        break;
    case Emoji:
        emoDict->remove_from_favoriteList(w);
        break;
    }
}

vector<Word*>& DictCollection::favoriteList() {
    switch (curDict) {
    case EngEng:
        return engEngDict->favoriteList;
        break;
    case VieEng:
        return vieEngDict->favoriteList;
        break;
    case EngVie:
        return engVieDict->favoriteList;
        break;
    default:
        return emoDict->favoriteList;
        break;
    }
}

vector<Word*>& DictCollection::searchHistory() {
    switch (curDict) {
    case EngEng:
        return engEngDict->searchHistory;
        break;
    case VieEng:
        return vieEngDict->searchHistory;
        break;
    case EngVie:
        return engVieDict->searchHistory;
        break;
    default:
        return emoDict->searchHistory;
        break;
    }
}

vector<pair<string, string>> DictCollection::randomQuiz() {
    switch (curDict) {
    case EngEng:
        return engEngDict->randomQuiz();
        break;
    case VieEng:
        return vieEngDict->randomQuiz();
        break;
    case EngVie:
        return engVieDict->randomQuiz();
        break;
    default:
        return emoDict->randomQuiz();
        break;
    }
}

vector<string> DictCollection::prefixMatch(const string& s, int max) {
    switch (curDict) {
    case EngEng:
        return engEngDict->prefixMatch(s, max);
        break;
    case VieEng:
        return vieEngDict->prefixMatch(s, max);
        break;
    case EngVie:
        return engVieDict->prefixMatch(s, max);
        break;
    default:
        return emoDict->prefixMatch(s, max);
        break;
    }
}

Word *DictCollection::insert(const string& s) {
    switch (curDict) {
    case EngEng:
        return engEngDict->insert(s);
        break;
    case VieEng:
        return vieEngDict->insert(s);
        break;
    case EngVie:
        return engVieDict->insert(s);
        break;
    default:
        return emoDict->insert(s);
        break;
    }
}
Word *DictCollection::insert(const string& s, const string& s2) {
    switch (curDict) {
    case EngEng:
        return engEngDict->insert(s, s2);
        break;
    case VieEng:
        return vieEngDict->insert(s, s2);
        break;
    case EngVie:
        return engVieDict->insert(s, s2);
        break;
    default:
        return emoDict->insert(s, s2);
        break;
    }
}

Word *DictCollection::insert(const Word& w) {
    switch (curDict) {
    case EngEng:
        return engEngDict->insert(w);
        break;
    case VieEng:
        return vieEngDict->insert(w);
        break;
    case EngVie:
        return engVieDict->insert(w);
        break;
    default:
        return emoDict->insert(w);
        break;
    }
}
