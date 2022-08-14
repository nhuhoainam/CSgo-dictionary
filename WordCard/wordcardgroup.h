#ifndef WORDCARDGROUP_H
#define WORDCARDGROUP_H

#include "api/dictionary_UI_testing.hpp"

#include <QWidget>
#include <list>
using std::list;

class WordCard;
class WordCardLayout;
class WordCardGroup : public QWidget
{
    Q_OBJECT
public:
    explicit WordCardGroup(QWidget *parent = nullptr);

    void addCard(Word, bool favorite);
    void clear();
    WordCard *getCard(const QString &keyword) const;
private:
    list<WordCard*> wordList;
    WordCardLayout *layout;
signals:
    void wordSelected(const QString&);
    void wordToggleFavorite(const QString &, bool on);
};

#endif // WORDCARDGROUP_H
