#include "wordcardgroup.h"
#include "wordcardlayout.h"
#include "wordcard.h"

WordCardGroup::WordCardGroup(QWidget *parent)
    : QWidget{parent}
{
    layout = new WordCardLayout;
    setLayout(layout);
}

void WordCardGroup::addCard(const QString &keyword,
                            const QString &meaning,
                            bool favorite) {
    auto *w = new WordCard;
    w->setKeyword("Hello");
    w->addDefinition("sljkgsakgj;aswdhg");
    w->addDefinition("sljkgsakgj;aswdhg");
    w->addDefinition("sljkgsakgj;aswdhg");
    connect(w, &WordCard::wordSelected, this, &WordCardGroup::wordSelected);
    connect(w, &WordCard::favoriteStateChanged, this, &WordCardGroup::wordToggleFavorite);
    wordList.push_back(w);
    layout->addWidget(w);
}

void WordCardGroup::clear() {
    wordList.clear();
    layout->clearWidgets();
}

WordCard *WordCardGroup::getCard(const QString &keyword) const {
    return nullptr;
}
