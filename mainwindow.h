#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "api/Word.h"
#include "api/Dictionary.hpp"
#include "api/converter.h"

#include <QWidget>
#include <vector>
#include <tuple>
using std::tuple;
using std::vector;

class History;
class FavoriteList;
class SingleWordView;
class DictionaryEditor;
class Game;
class QStackedWidget;
class Sidebar;
class Home;

typedef Dictionary<41, getid_EngEng, getchar_EngEng> EngEngDictionary;
typedef Dictionary<41, getid_EngEng, getchar_EngEng> EngVieDictionary;
typedef Dictionary<41, getid_EngEng, getchar_EngEng> VieEngDictionary;
typedef Dictionary<41, getidEmotion, getchar_Emotion> EmojiDictionary;

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
    enum DictType {
        EngEng,
        VieEng,
        EngVie,
        Emoji,
    };
    Ui::MainWindow *ui;
    Home *home;
    FavoriteList *favoriteList;
    History *history;
    DictionaryEditor *dictEditor;
    SingleWordView *wordViewer;
    Game *game;
    QStackedWidget *container;
    Sidebar *sidebar;
    EngEngDictionary *engEngDict;
    DictType curDict;

    void setupUI();
    void connectSignalAndSlot();
    vector<QString> getCompletionChoices(const QString &);
    void setupScene();

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
    void handleDictionaryChanged(const QString &);
    void handleWordFavoriteToggle(const QString &, bool);

    void handleQuizRequest();
};

#endif // MAINWINDOW_H
