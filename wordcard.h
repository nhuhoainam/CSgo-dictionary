#ifndef WORDCARD_H
#define WORDCARD_H

#include <QWidget>
#include <QIcon>
#include <QGraphicsDropShadowEffect>
#include <vector>

namespace Ui {
class WordCard;
}

class WordCard : public QWidget
{
    Q_OBJECT
public:
    explicit WordCard(QWidget *parent = nullptr);
    ~WordCard();

    void setKeyword(const QString &keyword);
    void addDefinition(const QString &def);
    void setDefinitions(std::vector<const QString &> defs);
    void setFavoriteState(bool);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    bool event(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
signals:
    void favoriteStateChanged(const QString &word, bool checked);
    void wordSelected(const QString &word);

private:
    Ui::WordCard *ui;
    QIcon on_icon;
    QIcon off_icon;
    bool hovered = false;
    bool checked = true;
    QGraphicsDropShadowEffect *effect;

    void setupUi();
    void connectSignalAndSlot();
};

#endif // WORDCARD_H
