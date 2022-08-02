#include "singlewordview.h"
#include "ui_singlewordview.h"

SingleWordView::SingleWordView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleWordView)
{
    ui->setupUi(this);

    connect(ui->widget,
            &WordViewer::deleteRequest,
            this,
            &SingleWordView::deleteRequest);
    connect(ui->widget,
            &WordViewer::editRequest,
            this,
            &SingleWordView::editRequest);
    connect(ui->widget,
            &WordViewer::favoriteToggle,
            this,
            &SingleWordView::favoriteToggle);
}

SingleWordView::~SingleWordView()
{
    delete ui;
}
