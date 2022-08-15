#include "game.h"
#include "ui_game.h"

#include <cstdio>
#include <ctime>
#include <iostream>

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    m_optionButtons = new QButtonGroup();
    m_optionButtons->addButton(ui->ans1, 1);
    m_optionButtons->addButton(ui->ans2, 2);
    m_optionButtons->addButton(ui->ans3, 3);
    m_optionButtons->addButton(ui->ans4, 4);

    ui->chooseTitle->setFont(QFont("Poppins", 24));
    // To block input when an answer is chosen
    blocker = new QWidget(this);
    blocker->setStyleSheet("background-color: rgba(61,61, 61, 0);");
    blocker->resize( width(), height());
    blocker->hide();

    emptyScreen = new QWidget(this);
    emptyScreen->setStyleSheet("background-color: rgba(61,61, 61, 100);");
    emptyScreen->resize(width(), height());
    emptyScreen->hide();

    connect(m_optionButtons, &QButtonGroup::idClicked, this, [=](int id) {
        checkAnswer(id);
        blocker->show();
    });
    ui->guessKeywordBtn->setStyleSheet("QPushButton{ border-image: url(:/images/img/keyword_art.png) 0 0 0 0 stretch stretch }");
    ui->guessMeaningBtn->setStyleSheet("QPushButton{ border-image: url(:/images/img/meaning_art.png) 0 0 0 0 stretch stretch }");
    connect(ui->guessKeywordBtn, &QPushButton::clicked, this, &Game::setKeywordGame);
    connect(ui->guessMeaningBtn, &QPushButton::clicked, this, &Game::setMeaningGame);
}

void Game::checkAnswer(int i) {
    if (m_curAns != i-1) {
        m_optionButtons->button(i)->setProperty("ansState", false);
        m_optionButtons->button(m_curAns + 1)->setProperty("ansState", true);
    } else {
        m_optionButtons->button(i)->setProperty("ansState", true);
    }
    setStyleSheet(STYLE);
}

Game::~Game()
{
    delete ui;
}

void Game::nextQuestion() {
    emit requestQSet();
    for (int i = 1; i <= 4; i++) {
        auto choice = ui->ans1;
        switch(i) {
        case 1:
            choice = ui->ans1;
            break;
        case 2:
            choice = ui->ans2;
            break;
        case 3:
            choice = ui->ans3;
            break;
        case 4:
            choice = ui->ans4;
            break;
        }
        choice->setText(
                    type == GuessKeyword
                    ? QString::fromStdString(m_curQSet[i-1].first)
                    : QString::fromStdString(m_curQSet[i-1].second));
        m_optionButtons->button(i)->setProperty("ansState", "");
    }
    ui->questionLabel->setText(
                type == GuessKeyword
                ? QString::fromStdString(m_curKey.second)
                : QString::fromStdString(m_curKey.first));
    blocker->hide();
    setStyleSheet(STYLE);
}

void Game::mousePressEvent(QMouseEvent *event) {
    if (!blocker->isHidden())
        nextQuestion();

    QWidget::mousePressEvent(event);
}

void Game::keyPressEvent(QKeyEvent *event) {
    if (!blocker->isHidden())
        nextQuestion();
    switch (event->key()) {
    case Qt::Key_1:
        checkAnswer(1);
        break;
    case Qt::Key_2:
        checkAnswer(2);
        break;
    case Qt::Key_3:
        checkAnswer(3);
        break;
    case Qt::Key_4:
        checkAnswer(4);
        break;
    }
}

void Game::resizeEvent(QResizeEvent *event) {
    auto size = event->size();
    blocker->resize(event->size());
    emptyScreen->resize(width(), height());
    QWidget::resizeEvent(event);
}

void Game::prevQuestion() {
}

void Game::setMeaningGame() {
    type = Type::GuessMeaning;
    nextQuestion();
    ui->stackedWidget->setCurrentIndex(0);
    ui->questionLabel->setFont(QFont("Playfair Display", 24, QFont::Bold));
    for (auto btn : m_optionButtons->buttons()) {
        btn->setFont(QFont("Poppins", 16));
    }

}
void Game::setKeywordGame() {
    type = Type::GuessKeyword;
    nextQuestion();
    ui->stackedWidget->setCurrentIndex(0);
    ui->questionLabel->setFont(QFont("Playfair Display", 24));
    for (auto btn : m_optionButtons->buttons()) {
        btn->setFont(QFont("Poppins", 24, QFont::Bold));
    }
}

void Game::start() {
    ui->stackedWidget->setCurrentIndex(1);
    blocker->hide();
}

void Game::setQSet(std::vector<std::pair<string, string> > payload) {
    for (int i = 0; i < 4; i++) {
        m_curQSet[i] = payload[i];
    }
    std::srand(std::time(NULL));
    m_curAns = rand() % 4;
    m_curKey = m_curQSet[m_curAns];
}
