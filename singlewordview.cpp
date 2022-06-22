#include "singlewordview.h"
#include "ui_singlewordview.h"

SingleWordView::SingleWordView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleWordView)
{
    ui->setupUi(this);
}

SingleWordView::~SingleWordView()
{
    delete ui;
}
