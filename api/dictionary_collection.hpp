#ifndef DICTIONARY_COLLECTION_HPP
#define DICTIONARY_COLLECTION_HPP

#include "Dictionary.hpp"
#include "converter.h"

typedef Dictionary<41, getid_EngEng, getchar_EngEng> EngEngDictionary;
typedef Dictionary<41, getid_EngEng, getchar_EngEng> EngVieDictionary;
typedef Dictionary<41, getid_EngEng, getchar_EngEng> VieEngDictionary;
typedef Dictionary<41, getidEmotion, getchar_Emotion> EmojiDictionary;

class DictCollection {
    EngEngDictionary *engEngDict;
    EngVieDictionary *engVieDict;
    VieEngDictionary *vieEngDict;
    EmojiDictionary *emoDict;

public:
    enum DictType {
        EngEng,
        VieEng,
        EngVie,
        Emoji,
    };
    DictType curDict;
    DictCollection();
    void init();
    void close();
    Word *find(const Word&);
    Word *query(const Word&);

    Word *insert(const string&);
    Word *insert(const string&, const string&);
    Word *insert(const Word&);

    bool erase(const Word&);
    bool erase(const string&);
    const Word* random_word() const;
    void add_to_favoriteList(Word*);
    void remove_from_favoriteList(Word*);
    vector<Word*>& favoriteList();
    vector<Word*>& searchHistory();
    vector<pair<string, string>> randomQuiz();
    vector<string> prefixMatch(const string&, int);
};

#endif
