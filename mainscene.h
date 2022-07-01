#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "searchbox.h"

#include <QWidget>
#include <QListWidget>
#include <tuple>
#include <vector>
using std::tuple;
using std::vector;

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
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void keyPressEvent(QKeyEvent *event);

protected:
    Ui::MainScene *ui;

private:
    SearchBox *searchBox;
    QListWidget *completePopup;

    void adjustPopup();
    void setupUi();
    void connectSignalAndSlot();
signals:
    void searchRequest(const QString&);
    void completionRequest(const QString&);
    void wordSelected(const QString&);
    void wordToggleFavorite(const QString&, bool);
};

#endif // MAINSCENE_H
