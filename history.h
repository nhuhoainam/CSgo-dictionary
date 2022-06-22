#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>

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

private:
    Ui::History *ui;
    SearchBox *searchBox;
};

#endif // HISTORY_H
