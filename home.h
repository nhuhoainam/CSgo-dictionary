#ifndef HOME_H
#define HOME_H

#include "searchbox.h"

#include <QWidget>
#include <QListWidget>

#include <tuple>
#include <vector>
using std::tuple;
using std::vector;

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

    void setWordList(vector<tuple<QString, QString, bool>>);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Home *ui;
    SearchBox *searchBox;
    QListWidget *completePopup;

    void adjustPopup();
    void setupUi();
    void connectSignalAndSlot();
signals:
    void refreshRequest();
    void searchRequest(const QString&);
    void completionRequest(const QString&);
    void wordSelected(const QString&);
    void wordToggleFavorite(const QString&, bool);
};

#endif // HOME_H
