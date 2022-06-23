#include "home.h"
#include "ui_home.h"
#include "wordcard.h"
#include "searchbox.h"
#include "wordcardlayout.h"

#include <iostream>

Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    for (int i = 0; i < 4; i++) {
        ui->wordGroup->addCard("heightist", "Someone who believes that people are superior to or inferior to others on account of their respective heights, or that people of differing heights have different moral qualities and intellectual capabilities.", true);
    }
    connect(ui->searchBox, &SearchBox::searchFinished,
            this, &Home::searchRequest);
    connect(ui->wordGroup, &WordCardGroup::wordSelected,
            this, &Home::wordSelected);
    connect(ui->wordGroup, &WordCardGroup::wordToggleFavorite,
            this, &Home::wordToggleFavorite);
    connect(ui->resetBtn, &QPushButton::clicked,
            this, &Home::refreshRequest);
}

Home::~Home()
{
    delete ui;
}

void Home::setWordList(vector<tuple<QString, QString, bool>> words) {
    ui->wordGroup->clear();
    for (auto item : words) {
        auto keyword = std::get<0>(item);
        auto meaning = std::get<1>(item);
        auto favorite = std::get<2>(item);
        ui->wordGroup->addCard(keyword, meaning, favorite);
    }
}
