#ifndef HOME_H
#define HOME_H

#include "searchbox.h"

#include <QWidget>
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

private:
    Ui::Home *ui;
    SearchBox *searchBox;
signals:
    void refreshRequest();
    void searchRequest(const QString&);
    void wordSelected(const QString&);
    void wordToggleFavorite(const QString&, bool);
};

#endif // HOME_H
