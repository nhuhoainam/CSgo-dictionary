#include "game.h"
#include "ui_game.h"

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

    // To block input when an answer is chosen
    blocker = new QWidget(this);
    blocker->setStyleSheet("background-color: rgba(61,61, 61, 0);");
    blocker->resize( width(), height());
    blocker->hide();

    startScreen = new QLabel("Press any key to start...", this);
    startScreen->setStyleSheet("background-color: rgba(61,61, 61, 100); color: white");
    startScreen->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    startScreen->setFont(QFont("Sans serif", 42, 100));
    startScreen->resize(width(), height());
    startScreen->show();

    emptyScreen = new QWidget(this);
    emptyScreen->setStyleSheet("background-color: rgba(61,61, 61, 100);");
    emptyScreen->resize(width(), height());
    emptyScreen->hide();

    connect(m_optionButtons, &QButtonGroup::idClicked, this, [=](int id) {
        checkAnswer(id);
        blocker->show();
    });
}

void Game::checkAnswer(int i) {

    if (m_answer != m_optionButtons->button(i)->text()) {
        int correctIndex = 1;
        for (int i = 1; i <= 4; i++) {
            if (m_optionButtons->button(i)->text() == m_answer) {
                correctIndex = i;
                break;
            }
        }

        m_optionButtons->button(correctIndex)->setStyleSheet("background-color: green;");
        m_optionButtons->button(i)->setStyleSheet("background-color: red");
    } else {
        m_optionButtons->button(i)->setStyleSheet("background-color: green");
    }
}

Game::~Game()
{
    delete ui;
}

void Game::nextQuestion() {
    if (!m_questionSets.empty()) {
        auto currentSet = m_questionSets.front();
        for (int i = 1; i <= 4; i++) {
            m_optionButtons->button(i)->setText(currentSet[i+1]);
        }
        ui->word->setText(currentSet[0]);
        m_answer = currentSet[1];
    }

    if (!m_questionSets.empty())
        m_questionSets.pop();

    startScreen->hide();
    blocker->hide();
    for (auto btn : m_optionButtons->buttons()) {
        btn->setStyleSheet("");
    }
}

void Game::addQuestionSets(std::vector<std::array<QString, 6>> questions) {
    for (const auto &a : questions) {
        m_questionSets.push(a);
    }
}

void Game::mousePressEvent(QMouseEvent *event) {
    nextQuestion();

    QWidget::mousePressEvent(event);
}

void Game::keyPressEvent(QKeyEvent *event) {
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
    startScreen->resize(event->size());
    emptyScreen->resize(width(), height());
    ui->btnLayout->setSpacing(event->size().width() / 15);
    QMargins newMargins(size.width()/15,
                       size.height()/10,
                       size.width()/15,
                       size.height()/10);
    ui->btnLayout->setContentsMargins(newMargins);
    QWidget::resizeEvent(event);
}

void Game::prevQuestion() {

}
