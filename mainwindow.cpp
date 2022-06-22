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

    container->addWidget(home);
    container->addWidget(favoriteList);
    container->addWidget(history);

    container->setCurrentIndex(1);

    ui->horizontalLayout->addWidget(container);
}

MainWindow::~MainWindow()
{
    delete ui;
}
