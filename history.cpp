#include "history.h"
#include "ui_history.h"
#include "searchbox.h"
#include "wordcardgroup.h"

History::History(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::History)
{
    ui->setupUi(this);

    connect(ui->searchBox, &SearchBox::searchFinished,
            this, &History::searchRequest);
    connect(ui->wordGroup, &WordCardGroup::wordSelected,
            this, &History::wordSelected);
    connect(ui->wordGroup, &WordCardGroup::wordToggleFavorite,
            this, &History::wordToggleFavorite);
}

History::~History()
{
    delete ui;
}
