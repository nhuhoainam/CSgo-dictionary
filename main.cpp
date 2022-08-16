#include "widget.h"
#include "sidebar.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QLabel>
#include <QHBoxLayout>
#include <QRect>

void setupSidebar(Widget &w, Sidebar *&sidebar) {
    w.setLayout(new QVBoxLayout());
    w.layout()->setContentsMargins(0,0,0,0);
    w.layout()->addWidget(sidebar);
    QIcon *dictIcon = new QIcon(":/images/img/CSgo.png");
    sidebar->setSidebarIcon(*dictIcon);
    sidebar->addAction(QString("Home"), QIcon(":/images/img/home.svg"));
    sidebar->addAction(QString("Favorite list"), QIcon(":/images/img/bookmarks.svg"));
    sidebar->addAction(QString("History"), QIcon(":/images/img/history.svg"));
    sidebar->addAction(QString("Edit dictionary"), QIcon(":/images/img/edit.svg"));
    sidebar->addAction(QString("Game"), QIcon(":/images/img/game.svg"));
    sidebar->addAction(QString("About us"), QIcon(":/images/img/people-outline.svg"));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QString("CSgo Dictionary"));
    w.setWindowIcon(QIcon(":/images/img/CSgo.png"));
    w.show();
    return a.exec();
}
