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
        ui->cardGroup->addCard("heightist", "Someone who believes that people are superior to or inferior to others on account of their respective heights, or that people of differing heights have different moral qualities and intellectual capabilities.", true);
    }
    connect(ui->cardGroup, &WordCardGroup::wordSelected, this, [=](const QString &word) {
        std::cout << word.toStdString() << " ";
    });
    connect(ui->resetBtn, &QPushButton::clicked, this, [=](bool) {
        std::cout << "RESET ";
    });
}

Home::~Home()
{
    delete ui;
}
