#include "wordviewer.h"
#include "ui_wordviewer.h"

#include <QPainter>

WordViewer::WordViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordViewer)
{
    ui->setupUi(this);

    connect(ui->deleteBtn,
            &QPushButton::clicked,
            this,
            [=]() {
        auto keyword = ui->keyword->text();
        emit deleteRequest(keyword);
    });
    connect(ui->editBtn,
            &QPushButton::clicked,
            this,
            [=]() {
        auto keyword = ui->keyword->text();
        emit editRequest(keyword);
    });
    connect(ui->favBtn,
            &QPushButton::clicked,
            this,
            [=](bool state) {
        auto keyword = ui->keyword->text();
        emit favoriteToggle(keyword, state);
    });
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

void WordViewer::setWord(const QString &key, std::vector<QString> defs, bool favState) {
    ui->keyword->setText(key);
    QLayoutItem *item;
    while (!(item = ui->defLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
    // TODO
    // Add definitions
}
