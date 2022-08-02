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
    sidebar->addAction(QString("Home"), QIcon(":/images/img/home.svg"));
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
            &SingleWordView::editRequest,
            this,
            &MainWindow::handleWordViewerEdit);
    connect(wordViewer,
            &SingleWordView::favoriteToggle,
            this,
            &MainWindow::handleWordViewerFavorite);
}


void MainWindow::handleHomeSearchRequest(const QString &keyword) {
    qDebug() << "Search " << keyword << " in home";
    container->setCurrentIndex(6);
    sidebar->setSelected(0, false);
}

void MainWindow::handleFavoriteListSearchRequest(const QString &keyword) {
    qDebug() << "Search " << keyword << " in favorite list";
}

void MainWindow::handleHistorySearchRequest(const QString &keyword) {
    qDebug() << "Search " << keyword << " in history";
}

void MainWindow::handleHomeWordSelected(const QString&keyword) {
    qDebug() << "Select " << keyword << " in home";
}

void MainWindow::handleFavoriteListWordSelected(const QString&keyword) {
    qDebug() << "Select " << keyword << " in favorite list";
}

void MainWindow::handleHistoryWordSelected(const QString&keyword) {
    qDebug() << "Select " << keyword << " in history";
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

void MainWindow::handleWordViewerEdit(const QString &keyword) {
    qDebug() << "Edit " << keyword;
}
void MainWindow::handleWordViewerDelete(const QString &keyword) {
    qDebug() << "Delete " << keyword;
}
void MainWindow::handleWordViewerFavorite(const QString &keyword, bool on) {
    QString state = on ? "on" : "off";
    qDebug() << "Set favorite to " << state << " " << keyword;
}
