#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QButtonGroup>
#include <QButtonGroup>
#include <QKeyEvent>
#include <QLabel>
#include <queue>
#include <stack>
#include <string>

const QString STYLE =
        "QPushButton {"
        "	border-style: solid;"
        "	border-width: 2px;"
        "	border-radius: 10px;"
        "	border-color: #FFDEBA;"
        "	font: bold 14px;"
        "	padding: 6px;"
        "}"
        "*[ansState=\"true\"] {"
        "	background-color: green;"
        "}"
        "*[ansState=\"false\"] {"
        "	background-color: red;"
        "}"
        "QPushButton:hover {"
        "	background-color: orange;"
        "}"
;

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    void addQuestionSets(std::vector<std::array<QString, 6>> questions);

private slots:
    void checkAnswer(int i);
    void nextQuestion();
    void prevQuestion();

public slots:

signals:
    void requestNewQuestionSets(int n=1);

private:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    QString m_answer;
    QButtonGroup *m_optionButtons;

    QLabel *startScreen;
    QWidget *emptyScreen;
    QWidget *blocker;
    std::vector<std::array<QString, 6>> m_polledQuestionSets;
    std::queue<std::array<QString, 6>> m_questionSets;
    int m_requestThreshold = 5;
    Ui::Game *ui;
};

#endif // GAME_H
