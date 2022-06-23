#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "home.h"
#include "favoritelist.h"
#include "history.h"
#include "dictionaryeditor.h"
#include "singlewordview.h"
#include "sidebar.h"
#include "game.h"

#include <QWidget>
#include <QStackedWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Home *home;
    FavoriteList *favoriteList;
    History *history;
    DictionaryEditor *dictEditor;
    SingleWordView *wordViewer;
    Game *game;
    QStackedWidget *container;
};

#endif // MAINWINDOW_H
