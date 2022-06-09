#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QButtonGroup>
#include <QButtonGroup>
#include <QKeyEvent>
#include <QLabel>
#include <queue>
#include <stack>

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();

private slots:
    void checkAnswer(int i);

public slots:
    void nextQuestionSet();

    void addQuestionSets(std::vector<std::array<QString, 6>> questions) {
        for (std::array<QString, 6> a : questions) {
            m_questionSets.push(a);
        }
    }

signals:
    void answerChosen(int option);
    void requestNextQuestionSets(int n=1);
    void correct();
    void wrong();
    void thresholdReached();
    void outOfQuestions();

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
