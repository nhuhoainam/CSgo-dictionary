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

void History::setWordList(vector<tuple<QString, QString, bool>> words) {
    ui->wordGroup->clear();
    for (auto item : words) {
        auto keyword = std::get<0>(item);
        auto meaning = std::get<1>(item);
        auto favorite = std::get<2>(item);
        ui->wordGroup->addCard(keyword, meaning, favorite);
    }
}
