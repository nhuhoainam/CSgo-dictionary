#ifndef WORDCARDGROUP_H
#define WORDCARDGROUP_H

#include "api/Word.h"

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

    void addCard(const Word&, bool favorite);
    void clear();
    WordCard *getCard(const QString &keyword) const;
private:
    list<WordCard*> wordList;
    WordCardLayout *layout;
signals:
    void wordSelected(const QString&);
    void wordToggleFavorite(const QString &, bool on);
    void wordFavorite(const QString &);
    void wordUnfavorite(const QString &);
};

#endif // WORDCARDGROUP_H
