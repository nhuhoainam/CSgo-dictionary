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
#include "api/Dictionary.hpp"

const QString STYLE =
        "QPushButton {"
        "	border-style: solid;"
        "	border-width: 2px;"
        "	border-radius: 10px;"
        "	border-color: #FFDEBA;"
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
        "	color: white;"
        "}"
;

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    enum Type {
        GuessMeaning,
        GuessKeyword,
    };
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    void start();
    void setQSet(std::vector<std::pair<string, string> > payload);

private slots:
    void checkAnswer(int i);
    void nextQuestion();
    void prevQuestion();
    void setMeaningGame();
    void setKeywordGame();

public slots:

signals:
    void requestQSet();

private:
    Type type;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    QButtonGroup *m_optionButtons;

    QWidget *emptyScreen;
    QWidget *blocker;
    std::array<std::pair<std::string, std::string>, 4> m_curQSet;
    std::pair<std::string, std::string> m_curKey;
    int m_curAns;
    Ui::Game *ui;
};

#endif // GAME_H
