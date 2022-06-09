#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QButtonGroup>
#include <vector>
#include <array>

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
    void nextQuestionSet(const QString &keyword,
                         const QString &answer,
                         const QString &option1,
                         const QString &option2,
                         const QString &option3,
                         const QString &option4);

signals:
    void answerChosen(int option);
    void requestNextQuestionSet();
    void correct();
    void wrong();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    QString m_answer;
    QButtonGroup *m_optionButtons;
    QWidget *blocker;
    std::vector<std::array<QString, 6>> m_history;
    Ui::Game *ui;
};

#endif // GAME_H
