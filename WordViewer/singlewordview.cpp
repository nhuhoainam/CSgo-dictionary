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
            &WordViewer::favoriteToggle,
            this,
            &SingleWordView::favoriteToggle);
    connect(ui->widget,
            &WordViewer::wordEdited,
            this,
            &SingleWordView::wordEdited);
}

SingleWordView::~SingleWordView()
{
    delete ui;
}

void SingleWordView::setWord(Word w) {
    ui->widget->setWord(w);
}
