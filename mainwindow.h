#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "home.h"
#include "favoritelist.h"
#include "history.h"
#include "dictionaryeditor.h"
#include "WordViewer/singlewordview.h"
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
    vector<QString> getCompletionChoices(const QString &);

private slots:
    void handleHomeFocus();
    void handleHomeRefresh();
    void handleHomeSearchRequest(const QString&);
    void handleHomeCompletionRequest(const QString&);
    void handleHomeWordSelected(const QString&);
    void handleHomeWordFavorite(const QString&, bool);

    void handleFavoriteListFocus();
    void handleFavoriteListSearchRequest(const QString&);
    void handleFavoriteListWordSelected(const QString&);
    void handleFavoriteListCompletionRequest(const QString&);
    void handleFavoriteListWordFavorite(const QString&, bool);

    void handleHistoryFocus();
    void handleHistorySearchRequest(const QString&);
    void handleHistoryCompletionRequest(const QString&);
    void handleHistoryWordSelected(const QString&);
    void handleHistoryWordFavorite(const QString&, bool);

    void handleEditorFocus();
    void handleEditorReset();
    void handleEditorAdd(const QString&, std::vector<std::pair<QString, QString> >);
    void handleGameFocus();

    void handleWordViewerEdit(Word);
    void handleWordViewerDelete(const QString&);
    void handleWordViewerFavorite(const QString&, bool on);

    void handleSearchRequest(const QString&);
};

#endif // MAINWINDOW_H
