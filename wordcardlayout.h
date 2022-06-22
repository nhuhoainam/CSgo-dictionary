#ifndef WORDCARDLAYOUT_H
#define WORDCARDLAYOUT_H

#include <QGridLayout>
#include <QWidget>

class WordCard;
class WordCardLayout : public QGridLayout
{
    Q_OBJECT
public:
    WordCardLayout();

    void addWidget(WordCard *widget);
    void clearWidgets();
signals:
    void wordSelected(const QString &word);
};

#endif // WORDCARDLAYOUT_H
