#include "wordcard.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QIcon>
#include <iostream>

WordCard::WordCard(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
}

WordCard::WordCard(QString word, QString definition, bool favorite, QWidget *parent)
    : QWidget{parent}
{
    effect = new QGraphicsDropShadowEffect(this);
    effect->setColor(Qt::gray);
    effect->setOffset(3,3);
    effect->setBlurRadius(10);
    effect->setEnabled(false);
    setGraphicsEffect(effect);
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);
    keyword = new QLabel(word, this);
    keyword->setFont(keywordFont);
    keyword->move(QPoint(20, 20));

    meaning = new QLabel(definition, this);
    meaning->setFont(textFont);
    meaning->move(QPoint(20, 75));
    meaning->setWordWrap(true);
    meaning->setAlignment(Qt::AlignTop);

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
    favBtn->setStyleSheet("border: none;");

    connect(favBtn, &QPushButton::clicked, this, [=](bool checked) {
        emit favoriteStateChanged(checked);
        if (checked)
            favBtn->setIcon(on_icon);
        else
            favBtn->setIcon(off_icon);
    });
}

void WordCard::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);


    QRect boundaryRect = rect();
    boundaryRect.adjust(painter.pen().width(),
                        painter.pen().width(),
                        -painter.pen().width(),
                        -painter.pen().width());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(boundaryRect, 10.0, 10.0);
    QWidget::paintEvent(event);
}

QSize WordCard::minimumSizeHint() const {
    return QSize(300, 100);
}

QSize WordCard::sizeHint() const {
    return QSize(400, 200);
}

void WordCard::resizeEvent(QResizeEvent* event) {
    QSize size = event->size();
    favBtn->move(size.width() - 60, 30);
    meaning->setFixedWidth(size.width() - 40);
}

bool WordCard::event(QEvent* event) {
    if (event->type() == QEvent::HoverEnter) {
        hovered = true;
        effect->setEnabled(true);
        update();
        return true;
    } else if (event->type() == QEvent::HoverLeave) {
        hovered = false;
        effect->setEnabled(false);
        update();
        return true;
    }

    return QWidget::event(event);
}

void WordCard::mousePressEvent(QMouseEvent*) {
    update();
    emit wordSelected(keyword->text());
}
void WordCard::mouseReleaseEvent(QMouseEvent*) {
    update();
}
