#ifndef FAVORITELIST_H
#define FAVORITELIST_H

#include <QWidget>

namespace Ui {
class FavoriteList;
}

class FavoriteList : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteList(QWidget *parent = nullptr);
    ~FavoriteList();

private:
    Ui::FavoriteList *ui;
};

#endif // FAVORITELIST_H
