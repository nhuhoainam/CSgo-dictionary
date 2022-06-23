#include "favoritelist.h"
#include "ui_favoritelist.h"

#include "wordcard.h"

FavoriteList::FavoriteList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavoriteList)
{
    ui->setupUi(this);

    connect(ui->searchBox, &SearchBox::searchFinished,
            this, &FavoriteList::searchRequest);
    connect(ui->wordGroup, &WordCardGroup::wordSelected,
            this, &FavoriteList::wordSelected);
    connect(ui->wordGroup, &WordCardGroup::wordToggleFavorite,
            this, &FavoriteList::wordToggleFavorite);
}

FavoriteList::~FavoriteList()
{
    delete ui;
}

SearchBox *FavoriteList::getSearchBox() const {
    return searchBox;
}

void FavoriteList::setWordList(vector<tuple<QString, QString, bool>> words) {
    ui->wordGroup->clear();
    for (auto item : words) {
        auto keyword = std::get<0>(item);
        auto meaning = std::get<1>(item);
        auto favorite = std::get<2>(item);
        ui->wordGroup->addCard(keyword, meaning, favorite);
    }
}
