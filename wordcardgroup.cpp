#include "wordcardgroup.h"
#include "wordcardlayout.h"
#include "wordcard.h"

WordCardGroup::WordCardGroup(QWidget *parent)
    : QWidget{parent}
{
    layout = new WordCardLayout;
    setLayout(layout);
}

void WordCardGroup::addCard(Word word, bool favorite) {
    auto *w = new WordCard;
    w->setKeyword(QString::fromStdString(word.word));
    for (const auto &p : word.data) {
        w->addDefinition(QString::fromStdString(p.first));
        w->setFavoriteState(favorite);
    }
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
