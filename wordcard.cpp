#include "wordcard.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QIcon>

WordCard::WordCard(QWidget *parent)
    : QWidget{parent}
{
}

WordCard::WordCard(QString word, QString definition, bool favorite, QWidget *parent)
    : QWidget{parent}
{
    keyword = new QLabel(word, this);
    keyword->setFont(keywordFont);
    keyword->move(QPoint(20, 20));

    meaning = new QLabel(definition, this);
    meaning->setFont(textFont);
    meaning->move(QPoint(20, 75));
    meaning->setWordWrap(true);

    favBtn = new QPushButton(this);
    on_icon = QIcon("bookmark_solid.svg");
    off_icon = QIcon("bookmark_hollow.svg");
    if (favorite)
        favBtn->setIcon(on_icon);
    else
        favBtn->setIcon(off_icon);
    favBtn->setCheckable(true);
    favBtn->setChecked(favorite);
    favBtn->setFlat(true);
    favBtn->move(320, 30);
    favBtn->setIconSize(QSize(24, 24));

    connect(favBtn, &QPushButton::clicked, this, [=](bool checked) {
        emit favoriteStateChanged(checked);
        if (checked)
            favBtn->setIcon(on_icon);
        else
            favBtn->setIcon(off_icon);
    });
}

void WordCard::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    QRect boundaryRect = rect();
    boundaryRect.adjust(painter.pen().width(), painter.pen().width(), -painter.pen().width(), -painter.pen().width());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(boundaryRect, 10.0, 10.0);
}

QSize WordCard::minimumSizeHint() const {
    return QSize(200, 100);
}

QSize WordCard::sizeHint() const {
    return QSize(400, 200);
}

void WordCard::resizeEvent(QResizeEvent* event) {
    QSize size = event->size();
    favBtn->move(size.width() - 60, 30);
}
