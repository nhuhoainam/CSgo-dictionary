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
    blocker->setStyleSheet("background-color: rgba(61,61, 61, 0);"); //just to see it
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
        emit answerChosen(id);
        blocker->show();
    });

    connect(this, &Game::thresholdReached, this, [=]() {
        emit requestNextQuestionSets(10);
    });

    connect(this, &Game::answerChosen, this, &Game::checkAnswer);
    connect(this, &Game::outOfQuestions, this, [=]() {
        emptyScreen->show();
    });
}

void Game::nextQuestionSet()
{
     std::cout << "SET TEXT" << std::endl;
    if (!m_questionSets.empty()) {
        auto currentSet = m_questionSets.front();
        for (int i = 1; i <= 4; i++) {
            m_optionButtons->button(i)->setText(currentSet[i+1]);
        }
        ui->word->setText(currentSet[0]);
        m_answer = currentSet[1];
    }

    for (auto btn : m_optionButtons->buttons()) {
        btn->setStyleSheet("");
    }

    if (!m_questionSets.empty())
        m_questionSets.pop();
    else {
        emit outOfQuestions();
    }
}

void Game::checkAnswer(int i) {

    if (m_answer != m_optionButtons->button(i)->text()) {
        emit wrong();

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
        emit correct();

        m_optionButtons->button(i)->setStyleSheet("background-color: green");
    }
}

Game::~Game()
{
    delete ui;
}

void Game::mousePressEvent(QMouseEvent *event) {
    if (!startScreen->isHidden()) {
        startScreen->hide();
        nextQuestionSet();
    }
    if (!blocker->isHidden()) {
        blocker->hide();
        nextQuestionSet();
    }

    for (auto btn : m_optionButtons->buttons()) {
        btn->setStyleSheet("");
    }
    QWidget::mousePressEvent(event);
}

void Game::keyPressEvent(QKeyEvent *event) {
    if (!blocker->isHidden()) {
        blocker->hide();
        nextQuestionSet();
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

void Game::resizeEvent(QResizeEvent *event) {
    blocker->resize(event->size());
    startScreen->resize(event->size());
    emptyScreen->resize(width(), height());
    ui->btnLayout->setSpacing(event->size().width() / 15);
    ui->btnLayout->setMargin(event->size().width() / 15);
    QWidget::resizeEvent(event);
}

