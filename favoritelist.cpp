#include "favoritelist.h"
#include "ui_favoritelist.h"

FavoriteList::FavoriteList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavoriteList)
{
    ui->setupUi(this);
}

FavoriteList::~FavoriteList()
{
    delete ui;
}
