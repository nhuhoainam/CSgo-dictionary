#ifndef WORDCARD_H
#define WORDCARD_H

#include <QWidget>
#include <QPushButton>

class WordCard : public QWidget
{
    Q_OBJECT

private:
    QIcon on_icon;
    QIcon off_icon;
    QPushButton *favBtn;
    QString word;
    QString definition;
    QColor keywordColor;
    QColor textColor;
    QColor borderColor;
    QFont keywordFont = QFont("Times, Serif", 28, QFont::Bold);
    QFont textFont = QFont("Sans", 12);

    void drawWord();
    void drawDefinition();
public:
    explicit WordCard(QWidget *parent = nullptr);
    explicit WordCard(QString word,
                      QString definition,
                      bool favorite = false,
                      QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void resizeEvent(QResizeEvent*) override;
signals:
    void favoriteStateChanged(bool checked);
};

#endif // WORDCARD_H
