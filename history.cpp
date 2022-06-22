#include "history.h"
#include "ui_history.h"
#include "searchbox.h"
#include "wordcard.h"

History::History(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::History)
{
    ui->setupUi(this);
}

History::~History()
{
    delete ui;
}
