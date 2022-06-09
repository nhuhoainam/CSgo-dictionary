#include "game.h"
#include "ui_game.h"

#include <QButtonGroup>
#include <QKeyEvent>

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
    blocker->setStyleSheet("background-color: rgba(61,61, 61, 0);"); //just to see it
    blocker->resize( width(), height());
    blocker->hide();

    connect(m_optionButtons, &QButtonGroup::idClicked, this, [=](int id) {
        emit answerChosen(id);
        blocker->show();
    });

    connect(this, &Game::answerChosen, this, &Game::checkAnswer);
}

void Game::nextQuestionSet(const QString &keyword,
                           const QString &answer,
                           const QString &option1,
                           const QString &option2,
                           const QString &option3,
                           const QString &option4)
{
    ui->word->setText(keyword);
    m_optionButtons->button(1)->setText(option1);
    m_optionButtons->button(2)->setText(option2);
    m_optionButtons->button(3)->setText(option3);
    m_optionButtons->button(4)->setText(option4);

    for (auto btn : m_optionButtons->buttons()) {
        btn->setStyleSheet("");
    }

    m_answer = answer;
}

void Game::checkAnswer(int i) {

    if (m_answer != m_optionButtons->button(i)->text()) {
        emit wrong();

        int correctIndex;
        for (int i = 1; i <= 4; i++) {
            if (m_optionButtons->button(i)->text() == m_answer) {
                correctIndex = i;
                break;
            }
        }

        m_optionButtons->button(correctIndex)->setStyleSheet("background-color: green;");
        m_optionButtons->button(i)->setStyleSheet("background-color: red");
    } else {
        emit correct();

        m_optionButtons->button(i)->setStyleSheet("background-color: green");
    }
}

Game::~Game()
{
    delete ui;
}

void Game::mousePressEvent(QMouseEvent *event) {
    if (!blocker->isHidden()) {
        blocker->hide();
    }

    for (auto btn : m_optionButtons->buttons()) {
        btn->setStyleSheet("");
    }
    QWidget::mousePressEvent(event);
}

void Game::keyPressEvent(QKeyEvent *event) {
    if (!blocker->isHidden()) {
        blocker->hide();
    }

    for (auto btn : m_optionButtons->buttons()) {
        btn->setStyleSheet("");
    }
    switch (event->key()) {
    case Qt::Key_1:
        emit answerChosen(1);
        break;
    case Qt::Key_2:
        emit answerChosen(2);
        break;
    case Qt::Key_3:
        emit answerChosen(3);
        break;
    case Qt::Key_4:
        emit answerChosen(4);
        break;
    }
}
