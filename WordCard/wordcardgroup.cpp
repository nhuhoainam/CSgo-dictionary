#include "WordCard/wordcardgroup.h"
#include "WordCard/wordcardlayout.h"
#include "WordCard/wordcard.h"

WordCardGroup::WordCardGroup(QWidget *parent)
    : QFrame{parent}
{
    layout = new FlowLayout(40, 30, 20);
    setLayout(layout);
}

void WordCardGroup::addCard(const Word &word, bool favorite) {
    auto *w = new WordCard;
    w->setKeyword(QString::fromStdString(word.word));
    for (const auto &p : word.data) {
        w->addDefinition(QString::fromStdString(p.first));
    }
    w->setFavoriteState(favorite);
    connect(w,
            &WordCard::wordSelected,
            this,
            &WordCardGroup::wordSelected);
    connect(w,
            &WordCard::favoriteStateChanged,
            this,
            &WordCardGroup::wordToggleFavorite);
    connect(w,
            &WordCard::favoriteStateChanged,
            this,
            [=](const QString& keyword, bool on) {
        if (on) {
            emit wordFavorite(keyword);
        } else {
            emit wordUnfavorite(keyword);
        }
    });
    wordList.push_back(w);
    layout->addWidget(w);
}

void WordCardGroup::clear() {
    wordList.clear();
    layout->clear();
}

WordCard *WordCardGroup::getCard(const QString &keyword) const {
    return nullptr;
}
