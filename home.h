#ifndef HOME_H
#define HOME_H

#include "searchbox.h"

#include <QWidget>

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

private:
    Ui::Home *ui;
    SearchBox *searchBox;
};

#endif // HOME_H
