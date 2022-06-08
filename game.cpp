#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    connect(ui->ans1, &QPushButton::clicked, [=]() {
        emit answerChosen(ui->ans1->text());
    });
    connect(ui->ans2, &QPushButton::clicked, [=]() {
        emit answerChosen(ui->ans2->text());
    });
    connect(ui->ans3, &QPushButton::clicked, [=]() {
        emit answerChosen(ui->ans3->text());
    });
    connect(ui->ans4, &QPushButton::clicked, [=]() {
        emit answerChosen(ui->ans4->text());
    });
}

Game::~Game()
{
    delete ui;
}
