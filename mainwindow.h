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
    Sidebar *sidebar;

    void setupUI();
    void connectSignalAndSlot();

private slots:
    void handleHomeFocus();
    void handleHomeSearchRequest(const QString&);
    void handleHomeWordSelected(const QString&);
    void handleHomeWordFavorite(const QString&, bool);

    void handleFavoriteListFocus();
    void handleFavoriteListSearchRequest(const QString&);
    void handleFavoriteListWordSelected(const QString&);
    void handleFavoriteListWordFavorite(const QString&, bool);

    void handleHistoryFocus();
    void handleHistorySearchRequest(const QString&);
    void handleHistoryWordSelected(const QString&);
    void handleHistoryWordFavorite(const QString&, bool);

    void handleEditorFocus();
    void handleEditorReset();
    void handleEditorAdd(const QString&, const QString&);
    void handleGameFocus();
};

#endif // MAINWINDOW_H
