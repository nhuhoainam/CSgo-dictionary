#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    setupUI();
    connectSignalAndSlot();
}

MainWindow::~MainWindow()
{
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
}

void MainWindow::handleSearchRequest(const QString& keyword) {
    // search word in dictionary
    // then set the word in singlewordview
    container->setCurrentIndex(6);

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
}
void MainWindow::handleFavoriteListWordFavorite(const QString &keyword, bool on) {
    QString state = on ? "on" : "off";
    qDebug() << "Toggle favorite " << keyword << state << " in favorite list";
}
void MainWindow::handleHistoryWordFavorite(const QString &keyword, bool on) {
    QString state = on ? "on" : "off";
    qDebug() << "Toggle favorite " << keyword << state << " in history";
}

void MainWindow::handleHomeFocus() {
    qDebug() << "Home focus";
}

void MainWindow::handleHomeRefresh() {
    qDebug() << "Home refresh request";
}

void MainWindow::handleFavoriteListFocus() {
    qDebug() << "Favorite list focus";
}
void MainWindow::handleHistoryFocus() {
    qDebug() << "History focus";
}

void MainWindow::handleEditorFocus() {
    dictEditor->resetDefEdit();
    qDebug() << "Editor focus";
}

void MainWindow::handleEditorReset() {
    qDebug() << "Reset dictionary";
}

void MainWindow::handleEditorAdd(const QString &keyword,
                                 std::vector<QString> defs) {
    qDebug() << "Add new word: " << keyword << " | ";
    for (auto def : defs) {
        qDebug() << def;
    }
}

void MainWindow::handleGameFocus() {
    qDebug() << "Game focus";
    game->start();
    game->addKeywordQSet({
                              { "Question", "1", "1", "2", "3", "4" },
                              { "Question2", "1", "1", "2", "3", "4" },
                          });
}

void MainWindow::handleWordViewerEdit(Word w) {
    qDebug() << "Edit " << QString::fromStdString(w.word);
    wordViewer->setWord(w);
}
void MainWindow::handleWordViewerDelete(const QString &keyword) {
    qDebug() << "Delete " << keyword;
}
void MainWindow::handleWordViewerFavorite(const QString &keyword, bool on) {
    QString state = on ? "on" : "off";
    qDebug() << "Set favorite to " << state << " " << keyword;
}

vector<QString> MainWindow::getCompletionChoices(const QString &word) {
    // search dictionary for similar word
    return {};
}

void MainWindow::handleHomeCompletionRequest(const QString& word) {
    // auto choices = getCompletionChoices(word);
    auto choices = {QString("Hello")};
    home->setCompletionChoices(choices);
}
void MainWindow::handleHistoryCompletionRequest(const QString& word) {
    auto choices = getCompletionChoices(word);
    favoriteList->setCompletionChoices(choices);
}
void MainWindow::handleFavoriteListCompletionRequest(const QString& word) {
    auto choices = getCompletionChoices(word);
    history->setCompletionChoices(choices);
}
