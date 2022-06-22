#ifndef FAVORITELIST_H
#define FAVORITELIST_H

#include "searchbox.h"

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
    SearchBox *searchBox;
};

#endif // FAVORITELIST_H
