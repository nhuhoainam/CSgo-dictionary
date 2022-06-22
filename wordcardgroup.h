#ifndef WORDCARDGROUP_H
#define WORDCARDGROUP_H

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

    void addCard(const QString &keyword,
                 const QString &meaning,
                 bool favorite);
    void clear();
    WordCard *getCard(const QString &keyword) const;
private:
    list<WordCard*> wordList;
    WordCardLayout *layout;

signals:
    void wordSelected(const QString &word);
};

#endif // WORDCARDGROUP_H
