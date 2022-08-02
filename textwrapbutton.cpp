#include "textwrapbutton.h"

#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

TextWrapButton::TextWrapButton(QWidget *parent) : QPushButton(parent) {
    label = new QLabel;
    setLayout(new QVBoxLayout);
    layout()->addWidget(label);
    layout()->setContentsMargins(0,0,0,0);
    label->setWordWrap(true);
    label->setMouseTracking(false);
    label->setTextInteractionFlags(Qt::NoTextInteraction);
}

TextWrapButton::TextWrapButton(const QString &text,
                               QWidget *parent)
    : QPushButton(text, parent)
{
    label = new QLabel(text);
    setLayout(new QVBoxLayout);
    layout()->addWidget(label);
    layout()->setContentsMargins(0,0,0,0);
    label->setWordWrap(true);
    label->setMouseTracking(false);
    label->setTextInteractionFlags(Qt::NoTextInteraction);
}

void TextWrapButton::setText(const QString &text) {
    qDebug() << "SET TEXT";
    label->setText(text);
}

QString TextWrapButton::text() const {
    return label->text();
}

void TextWrapButton::paintEvent(QPaintEvent *pe)
{
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(
    QStyle::PE_Widget, &o, &p, this);
};

void TextWrapButton::resizeEvent(QResizeEvent*event) {
    qDebug() << "RESIZE";
    QPushButton::resizeEvent(event);
    label->resize(event->size());
}
