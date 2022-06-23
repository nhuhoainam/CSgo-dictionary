#include "wordviewer.h"
#include "ui_wordviewer.h"

#include <QPainter>

WordViewer::WordViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordViewer)
{
    ui->setupUi(this);
}

WordViewer::~WordViewer()
{
    delete ui;
}

void WordViewer::paintEvent(QPaintEvent* event) {
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
