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
    ui->ans1->setText("asdlgkna  sldgks  sldsdgkl slgkd sklg slkdgans;g sgls slkgn lssl lsgln lsgknsl sdgjknbsl k skfdgja;s sjkgd sdjg"
                      "ASJKLGFBNASKG;sdljkg dsg");

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

    if (m_answer != m_optionButtons->button(i)->text()) {
        int correctIndex = 1;
        for (int i = 1; i <= 4; i++) {
            if (m_optionButtons->button(i)->text() == m_answer) {
                correctIndex = i;
                break;
            }
        }

        std::cout << "CORRECT" << std::endl;
        m_optionButtons->button(correctIndex)->setProperty("ansState", true);
        m_optionButtons->button(i)->setProperty("ansState", false);
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
    if (m_questionSets.empty())
        return;

    auto currentSet = m_questionSets.front();
    for (int i = 1; i <= 4; i++) {
        m_optionButtons->button(i)->setText(currentSet[i+1]);
        m_optionButtons->button(i)->setProperty("ansState", "");
    }
    ui->questionLabel->setText(currentSet[0]);
    m_answer = currentSet[1];

    m_questionSets.pop();

    blocker->hide();
    setStyleSheet(STYLE);
}

void Game::addMeaningQSet(std::vector<std::array<QString, 6>> questions) {
    for (const auto &a : questions) {
        m_questionSets.push(a);
    }
}

void Game::addKeywordQSet(std::vector<std::array<QString, 6>> questions) {
    for (const auto &a : questions) {
        m_questionSets.push(a);
    }
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
    ui->stackedWidget->setCurrentIndex(0);
    ui->questionLabel->setFont(QFont("Sans", 24, QFont::Bold));
    for (auto btn : m_optionButtons->buttons()) {
        btn->setFont(QFont("Sans", 16));
    }

}
void Game::setKeywordGame() {
    type = Type::GuessKeyword;
    ui->stackedWidget->setCurrentIndex(0);
    ui->questionLabel->setFont(QFont("Sans", 24));
    for (auto btn : m_optionButtons->buttons()) {
        btn->setFont(QFont("Sans", 24, QFont::Bold));
    }
}

void Game::start() {
    type = Type::GuessMeaning;
    ui->stackedWidget->setCurrentIndex(1);
    blocker->hide();
    while (!m_questionSets.empty())
        m_questionSets.pop();
    nextQuestion();
}
