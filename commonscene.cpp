#include "commonscene.h"
#include "wordcard.h"
#include "wordcardgroup.h"
#include "ui_mainscene.h"

CommonScene::CommonScene(QWidget *parent) :
    MainScene(parent)
{
    setContent(new WordCardGroup);
    for (int i = 0; i < 4; i++) {
        ui->content->addCard("heightist", "Someone who believes that people are superior to or inferior to others on account of their respective heights, or that people of differing heights have different moral qualities and intellectual capabilities.", true);
    }
    for (int i = 0; i < 4; i++) {
        ui->content->addCard("heightist", "Someone who believes that people are superior to or inferior to others on account of their respective heights, or that people of differing heights have different moral qualities and intellectual capabilities.", true);
    }
    connect(ui->wordGroup, &WordCardGroup::wordSelected,
            this, &MainScene::wordSelected);
    connect(ui->wordGroup, &WordCardGroup::wordToggleFavorite,
            this, &MainScene::wordToggleFavorite);
}

void MainScene::setWordList(vector<tuple<QString, QString, bool>> words) {
    ui->content->clear();
    for (auto item : words) {
        auto keyword = std::get<0>(item);
        auto meaning = std::get<1>(item);
        auto favorite = std::get<2>(item);
        ui->content->addCard(keyword, meaning, favorite);
    }
}
