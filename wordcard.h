#ifndef WORDCARD_H
#define WORDCARD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

class WordCard : public QWidget
{
    Q_OBJECT

private:
    QIcon on_icon;
    QIcon off_icon;

    QColor keywordColor;
    QColor textColor;
    QColor borderColor;

    QFont keywordFont = QFont("Times, Serif", 24, QFont::Bold);
    QFont textFont = QFont("Sans", 11);

    QPushButton *favBtn;
    QLabel *keyword;
    QLabel *meaning;

    bool hovered = false;
    QGraphicsDropShadowEffect *effect;
public:
    explicit WordCard(QWidget *parent = nullptr);
    explicit WordCard(QString word,
                      QString definition,
                      bool favorite = false,
                      QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    bool event(QEvent*) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
signals:
    void favoriteStateChanged(bool checked);
    void wordSelected(const QString &word);
};

#endif // WORDCARD_H
