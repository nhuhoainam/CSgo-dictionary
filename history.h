#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include <tuple>
#include <vector>
using std::tuple;
using std::vector;

class SearchBox;
namespace Ui {
class History;
}

class History : public QWidget
{
    Q_OBJECT

public:
    explicit History(QWidget *parent = nullptr);
    ~History();
    void setWordList(vector<tuple<QString, QString, bool>>);

private:
    Ui::History *ui;
signals:
    void searchRequest(const QString&);
    void wordSelected(const QString&);
    void wordToggleFavorite(const QString&, bool);
};

#endif // HISTORY_H
