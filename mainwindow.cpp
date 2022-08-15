#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Scene/home.h"
#include "Scene/favoritelist.h"
#include "Scene/history.h"
#include "dictionaryeditor.h"
#include "WordViewer/singlewordview.h"
#include "sidebar.h"
#include "api/Dictionary.hpp"
#include "game.h"

#include <QStackedWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    dict.init();
    setupUI();
    connectSignalAndSlot();

    // TODO
    // Initalize dicts and assign curDict
    setupScene();
}

MainWindow::~MainWindow()
{
    dict.close();
    delete ui;
}

void MainWindow::setupUI() {
    ui->setupUi(this);
    container = new QStackedWidget;
    container->setObjectName("container");
    container->setStyleSheet("#container {background-color: rgb(255, 248, 239);}");

    favoriteList = new FavoriteList;
    history = new History;
    home = new Home;
    dictEditor = new DictionaryEditor;
    wordViewer = new SingleWordView;
    game = new Game;
    sidebar = new Sidebar;

    container->addWidget(home);
    container->addWidget(favoriteList);
    container->addWidget(history);
    container->addWidget(dictEditor);
    container->addWidget(game);
    // placeholder
    container->addWidget(new QWidget);
    container->addWidget(wordViewer);

    ui->horizontalLayout->setContentsMargins(0,0,0,0);
    ui->horizontalLayout->addWidget(sidebar);
    QIcon *dictIcon = new QIcon(":/images/img/CSgo.png");
    sidebar->setSidebarIcon(*dictIcon);
    sidebar->addDefaultAction(QString("Home"), QIcon(":/images/img/home.svg"));
    sidebar->addAction(QString("Favorite list"), QIcon(":/images/img/bookmarks.svg"));
    sidebar->addAction(QString("History"), QIcon(":/images/img/history.svg"));
    sidebar->addAction(QString("Edit dictionary"), QIcon(":/images/img/edit.svg"));
    sidebar->addAction(QString("Game"), QIcon(":/images/img/game.svg"));
    sidebar->addAction(QString("About us"), QIcon(":/images/img/people-outline.svg"));

    container->setCurrentIndex(0);
    ui->horizontalLayout->addWidget(container);
}

void MainWindow::connectSignalAndSlot() {
    connect(sidebar,
            &Sidebar::actionSelected,
            container,
            &QStackedWidget::setCurrentIndex);

    connect(sidebar,
            &Sidebar::actionSelected,
            this,
            [=](int index) {
        switch (index) {
        case 0:
            handleHomeFocus();
            break;
        case 1:
            handleFavoriteListFocus();
            break;
        case 2:
            handleHistoryFocus();
            break;
        case 3:
            handleEditorFocus();
            break;
        case 4:
            handleGameFocus();
            break;
        }
    });

    connect(favoriteList,
            &FavoriteList::searchRequest,
            this,
            &MainWindow::handleFavoriteListSearchRequest);
    connect(favoriteList,
            &FavoriteList::wordSelected,
            this,
            &MainWindow::handleFavoriteListWordSelected);
    connect(favoriteList,
            &FavoriteList::wordToggleFavorite,
            this,
            &MainWindow::handleFavoriteListWordFavorite);
    connect(favoriteList,
            &FavoriteList::completionRequest,
            this,
            &MainWindow::handleFavoriteListCompletionRequest);
    connect(history,
            &History::searchRequest,
            this,
            &MainWindow::handleHistorySearchRequest);
    connect(history,
            &History::wordSelected,
            this,
            &MainWindow::handleHistoryWordSelected);
    connect(history,
            &History::wordToggleFavorite,
            this,
            &MainWindow::handleHistoryWordFavorite);
    connect(history,
            &History::completionRequest,
            this,
            &MainWindow::handleHistoryCompletionRequest);

    connect(home,
            &Home::wordSelected,
            this,
            &MainWindow::handleHomeWordSelected);
    connect(home,
            &Home::searchRequest,
            this,
            &MainWindow::handleHomeSearchRequest);
    connect(home,
            &Home::wordToggleFavorite,
            this,
            &MainWindow::handleHomeWordFavorite);
    connect(home,
            &Home::refreshRequest,
            this,
            &MainWindow::handleHomeRefresh);
    connect(home,
            &Home::completionRequest,
            this,
            &MainWindow::handleHomeCompletionRequest);

    connect(dictEditor,
            &DictionaryEditor::addNewWord,
            this,
            &MainWindow::handleEditorAdd);
    connect(dictEditor,
            &DictionaryEditor::resetDictionary,
            this,
            &MainWindow::handleEditorReset);

    connect(wordViewer,
            &SingleWordView::deleteRequest,
            this,
            &MainWindow::handleWordViewerDelete);
    connect(wordViewer,
            &SingleWordView::wordEdited,
            this,
            &MainWindow::handleWordViewerEdit);
    connect(wordViewer,
            &SingleWordView::favoriteToggle,
            this,
            &MainWindow::handleWordViewerFavorite);
    connect(wordViewer,
            &SingleWordView::searchRequest,
            this,
            &MainWindow::handleSearchRequest);
    connect(wordViewer,
            &SingleWordView::completionRequest,
            this,
            &MainWindow::handleWordViewerCompletionRequest);

    connect(favoriteList,
            &MainScene::dictionaryTypeChange,
            this,
            &MainWindow::handleDictionaryChanged);
    connect(history,
            &MainScene::dictionaryTypeChange,
            this,
            &MainWindow::handleDictionaryChanged);
    connect(home,
            &Home::dictionaryTypeChange,
            this,
            &MainWindow::handleDictionaryChanged);
    connect(game,
            &Game::requestQSet,
            this,
            &MainWindow::handleQuizRequest);
}

void MainWindow::setupScene() {
    // TODO
    // set up Home, FavoriteList and History using current dictionary
    // get data from curDict and use setWordLists for setup
    qDebug() << "Setup Scene";
    vector<pair<Word, bool>> homeList;
    vector<pair<Word, bool>> favList;
    vector<pair<Word, bool>> historyList;
    homeWordLists.clear();
    for (int i = 0; i < 8; i++) {
        const Word *w = dict.random_word();
        homeList.push_back({*w, w->isFavorite});
        homeWordLists.push_back(w->word);

        if (i < dict.favoriteList().size()) {
        const Word *f = dict.favoriteList()[i];
        favList.push_back({*f, true});
        }

        if (i < dict.searchHistory().size()) {
        const Word *h = dict.searchHistory()[i];
        historyList.push_back({*h, false});
        }
    }
    home->setWordList(homeList);
    favoriteList->setWordList(favList);
    history->setWordList(historyList);
}

void MainWindow::handleDictionaryChanged(const QString &type) {
    if (type == "English - English") {
        dict.curDict = DictCollection::EngEng;
    } else if (type == "Vietnamese - English") {
        dict.curDict = DictCollection::VieEng;
    } else if (type == "English - Vietnamese") {
        dict.curDict = DictCollection::EngVie;
    } else if (type == "Emotional") {
        dict.curDict = DictCollection::Emoji;
    } else if (type == "Slang") {
        dict.curDict = DictCollection::Slang;
    }
    handleHomeRefresh();
    home->changeDictionary(type);
    favoriteList->changeDictionary(type);
    history->changeDictionary(type);
    setupScene();
}

void MainWindow::handleSearchRequest(const QString& keyword) {
    // TODO
    // search word in dictionary
    // then set the word in singlewordview
    string s = keyword.toStdString();
    auto *q = dict.find(s);
    curEditedWord = q->word;
    container->setCurrentIndex(6);
    handleWordViewerFocus();

    // Deselect the sidebar to avoid confusion
    sidebar->setSelected(0, false);
    sidebar->setSelected(1, false);
    sidebar->setSelected(2, false);
}

void MainWindow::handleHomeSearchRequest(const QString &keyword) {
    handleSearchRequest(keyword);
}

void MainWindow::handleFavoriteListSearchRequest(const QString &keyword) {
    handleSearchRequest(keyword);
}

void MainWindow::handleHistorySearchRequest(const QString &keyword) {
    handleSearchRequest(keyword);
}

void MainWindow::handleHomeWordSelected(const QString&keyword) {
    handleSearchRequest(keyword);
}

void MainWindow::handleFavoriteListWordSelected(const QString&keyword) {
    handleSearchRequest(keyword);
}

void MainWindow::handleHistoryWordSelected(const QString&keyword) {
    handleSearchRequest(keyword);
}

void MainWindow::handleHomeWordFavorite(const QString &keyword, bool on) {
    QString state = on ? "on" : "off";
    qDebug() << "Toggle favorite " << keyword << state << " in home";
    handleWordFavoriteToggle(keyword, on);
}

void MainWindow::handleFavoriteListWordFavorite(const QString &keyword, bool on) {
    QString state = on ? "on" : "off";
    qDebug() << "Toggle favorite " << keyword << state << " in favorite list";
    handleWordFavoriteToggle(keyword, on);
}

void MainWindow::handleHistoryWordFavorite(const QString &keyword, bool on) {
    QString state = on ? "on" : "off";
    qDebug() << "Toggle favorite " << keyword << state << " in history";
    handleWordFavoriteToggle(keyword, on);
}

void MainWindow::handleHomeFocus() {
    qDebug() << "Home focus";
    vector<pair<Word, bool>> ls;
    for (const auto &s : homeWordLists) {
        Word *word = dict.find(s);
        ls.push_back({*word, word->isFavorite});
    }
    home->setWordList(ls);
}

void MainWindow::handleHomeRefresh() {
    vector<pair<Word, bool>> ls;
    QSize size = home->size();
    int n = size.width() / 80;
    for (int i = 0; i < n; i++) {
        const Word *w = dict.random_word();
        ls.push_back({*w, w->isFavorite});
        homeWordLists.push_back(w->word);
    }
    home->setWordList(ls);
}

void MainWindow::handleFavoriteListFocus() {
    vector<pair<Word, bool>> ls;
    for (Word *w : dict.favoriteList()) {
        if (!w)
            return;
        ls.push_back({*w, true});
    }
    favoriteList->setWordList(ls);
}

void MainWindow::handleHistoryFocus() {
    int count = 0;
    vector<pair<Word, bool>> ls;
    for (auto *w : dict.searchHistory()) {
        count++;
        if (count >= 50)
            break;
        ls.push_back({*w, w->isFavorite});
    }
    history->setWordList(ls);
}

void MainWindow::handleEditorFocus() {
    dictEditor->resetDefEdit();
    qDebug() << "Editor focus";
}

void MainWindow::handleEditorReset() {
    qDebug() << "Reset dictionary";
    dict.reset();
    setupScene();
}

void MainWindow::handleEditorAdd(const QString &keyword,
                                 std::vector<std::pair<QString, QString> > defs) {
    qDebug() << "Add new word: " << keyword << " | ";
    Word newWord(keyword.toStdString());
    std::vector<std::pair<std::string, std::string> > newDefs;
    for (auto &def : defs) {
        newDefs.push_back(std::pair(def.first.toStdString(), def.second.toStdString()));
        qDebug() << def.first << " + " << def.second;
    }
    newWord.data = newDefs;
    dict.insert(newWord);
}

void MainWindow::handleGameFocus() {
    qDebug() << "Game focus";
    game->start();
}

void MainWindow::handleWordViewerEdit(Word w) {
    qDebug() << "Edit " << QString::fromStdString(w.word);
    Word *word = dict.find(w);
    if (!word)
        return;

    *word = w;
    wordViewer->setWord(w);
}
void MainWindow::handleWordViewerDelete(const QString &keyword) {
    qDebug() << "Delete " << keyword;
}
void MainWindow::handleWordViewerFavorite(const QString &keyword, bool on) {
    QString state = on ? "on" : "off";
    qDebug() << "Set favorite to " << state << " " << keyword;
    handleWordFavoriteToggle(keyword, on);
}

vector<QString> MainWindow::getCompletionChoices(const QString &word) {
    // search dictionary for similar word
    vector<QString> choices;
    auto rs = dict.prefixMatch(word.toStdString(), 8);
    for (const string& s : rs) {
        choices.push_back(QString::fromStdString(s));
    }
    return choices;
}

void MainWindow::handleHomeCompletionRequest(const QString& word) {
    // auto choices = getCompletionChoices(word);
    vector<QString> choices;
    auto rs = dict.prefixMatch(word.toStdString(), 8);
    for (const string& s : rs) {
        choices.push_back(QString::fromStdString(s));
    }
    home->setCompletionChoices(choices);
}

void MainWindow::handleWordViewerCompletionRequest(const QString &word) {
    auto choices = getCompletionChoices(word);
    wordViewer->setCompletionChoices(choices);
}
void MainWindow::handleHistoryCompletionRequest(const QString& word) {
    auto choices = getCompletionChoices(word);
    history->setCompletionChoices(choices);
}
void MainWindow::handleFavoriteListCompletionRequest(const QString& word) {
    auto choices = getCompletionChoices(word);
    favoriteList->setCompletionChoices(choices);
}

void MainWindow::handleWordFavoriteToggle(const QString &keyword, bool state) {
    Word *word = dict.find(keyword.toStdString());
    if (!word)
        return;
    if (state == true) {
        dict.add_to_favoriteList(word);
    } else {
        dict.remove_from_favoriteList(word);
    }
    qDebug() << "Current Favorite List";
    for (auto w : dict.favoriteList()) {
        qDebug() << QString::fromStdString(w->word);
    }
}

void MainWindow::handleQuizRequest() {
    std::vector<std::pair<std::string, std::string> > payload = dict.randomQuiz();
    game->setQSet(payload);
    qDebug() << "Question set generated:\n";
    for (int i = 0; i < 4; i++)
        qDebug() << QString::fromStdString(payload[i].first) << " " << QString::fromStdString(payload[i].second) << "\n";
}

void MainWindow::handleWordViewerFocus() {
    Word *word = dict.find(curEditedWord);
    if (!word) {
        return;
    }

    wordViewer->setWord(*word);
}
