#include "wordcardlayout.h"
#include "wordcard.h"

WordCardLayout::WordCardLayout() : QGridLayout()
{
    setSpacing(30);
}

void WordCardLayout::addWidget(WordCard *widget) {
    QGridLayout::addWidget(widget, count()/2, count()%2);
}

void WordCardLayout::clearWidgets() {
    QLayoutItem *item;
    while ((item = takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}
