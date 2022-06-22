#include <QIcon>
#include <QPixmap>

#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
//    setWindowIcon(QIcon(":/resources/img/icon.png"));
//    QPixmap hero(":/resources/img/icon2.png");
//    int w = ui->hero_pic->width(),
//        h = ui->hero_pic->height();
//    ui->hero_pic->setPixmap(hero.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

Widget::~Widget()
{
    delete ui;
}

