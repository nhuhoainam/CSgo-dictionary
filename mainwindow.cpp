#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
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
    Sidebar *sidebar = new Sidebar;

    container->addWidget(home);
    container->addWidget(favoriteList);
    container->addWidget(history);
    container->addWidget(dictEditor);
    container->addWidget(game);
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
    connect(sidebar,
            &Sidebar::actionSelected,
            container,
            &QStackedWidget::setCurrentIndex);

    ui->horizontalLayout->addWidget(container);
}

MainWindow::~MainWindow()
{
    delete ui;
}
