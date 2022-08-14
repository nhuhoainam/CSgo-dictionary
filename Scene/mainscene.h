#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "searchbox.h"
#include "api/dictionary_UI_testing.hpp"

#include <QWidget>
#include <QListWidget>
#include <tuple>
#include <vector>
using std::tuple;
using std::vector;

class Dictionary;

namespace Ui {
class MainScene;
}

class MainScene : public QWidget
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = nullptr);
    explicit MainScene(QString title, QWidget *parent = nullptr);
    ~MainScene();

    void setWordList(vector<tuple<QString, QString, bool>>);
    void setWordList(vector<pair<Word, bool>>);
    void setCompletionChoices(vector<QString>);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void keyPressEvent(QKeyEvent *event);

protected:
    Ui::MainScene *ui;

private:
    QListWidget *completePopup;

    void adjustPopup();
    void setupUi();
    void connectSignalAndSlot();
    void handleSearch(const QString &text);
signals:
    void dictionaryTypeChange(const QString&);
    void searchRequest(const QString&);
    void completionRequest(const QString&);
    void wordSelected(const QString&);
    void wordToggleFavorite(const QString&, bool);
};

#endif // MAINSCENE_H
