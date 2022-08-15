#ifndef HOME_H
#define HOME_H

#include "mainscene.h"

class Home : public MainScene
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
private:
    QPushButton *refreshBtn;
    void setupRefreshBtn();
signals:
    void refreshRequest();
};


#endif // HOME_H
