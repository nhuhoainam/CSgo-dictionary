#ifndef WORDCARDLAYOUT_H
#define WORDCARDLAYOUT_H

#include <QGridLayout>
#include <QWidget>

class WordCard;
class WordcardLayout : public QGridLayout
{
    Q_OBJECT
public:
    WordcardLayout();

    void addWidget(WordCard *widget);
    void clearWidgets();
signals:
    void wordSelected(const QString &word);
};

#endif // WORDCARDLAYOUT_H
