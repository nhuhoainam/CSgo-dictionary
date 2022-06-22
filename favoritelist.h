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

    SearchBox *getSearchBox() const;

private:
    Ui::FavoriteList *ui;
    SearchBox *searchBox;
signals:
    void searchRequest(const QString&);
    void wordSelected(const QString&);
    void wordToggleFavorite(const QString&, bool on);
};

#endif // FAVORITELIST_H
