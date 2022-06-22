#include "wordcardlayout.h"

#include "wordcard.h"

WordcardLayout::WordcardLayout() : QGridLayout()
{
}

void WordcardLayout::addWidget(WordCard *widget) {
    connect(widget, &WordCard::wordSelected, this, &WordcardLayout::wordSelected);
    QGridLayout::addWidget(widget, count()/2, count()%2);
}

void WordcardLayout::clearWidgets() {
    QLayoutItem *item;
    while ((item = takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}
