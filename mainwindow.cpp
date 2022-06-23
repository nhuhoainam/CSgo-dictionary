#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    container = new QStackedWidget;

    favoriteList = new FavoriteList;
    history = new History;
    home = new Home;
    dictEditor = new DictionaryEditor;
    wordViewer = new SingleWordView;

    container->addWidget(home);
    container->addWidget(favoriteList);
    container->addWidget(history);
    container->addWidget(dictEditor);
    container->addWidget(wordViewer);

    container->setCurrentIndex(4);

    ui->horizontalLayout->addWidget(container);
}

MainWindow::~MainWindow()
{
    delete ui;
}
