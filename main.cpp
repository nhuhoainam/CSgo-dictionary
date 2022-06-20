#include "widget.h"
#include "sidebar.h"

#include <QApplication>
#include <QLocale>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    Sidebar* sidebar = new Sidebar();

    w.setWindowTitle("CSgo Dictionary");
    w.setLayout(new QVBoxLayout());
    w.layout()->setContentsMargins(0,0,0,0);
    w.layout()->setSpacing(0);
    w.layout()->addWidget(sidebar);

    sidebar->setMenuWidth(200);
    sidebar->setMenuIcon(QIcon(":/images/img/CSgo.png"));
    sidebar->addMenuAction(
                QIcon(QPixmap(":/images/img/home_icon.png").scaled(
                          QSize(12, 12),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation)),
                "Home"); // Does not work
    sidebar->addMenuAction(QIcon(":/images/img/bookmarks_icon.png"), "Favorite List");
    sidebar->addMenuAction(QIcon(":/images/img/history_icon.png"), "History");
    sidebar->addMenuAction(QIcon(":/images/img/edit_icon.png"), "Edit dictionary");
    sidebar->addMenuAction(QIcon(":/images/img/cards_icon.png"), "Game");
    sidebar->setIconSize(QSize(24, 24));

    w.setStyleSheet("MainMenu                  { background-color: #FFFFFF;   }"
                    "#MainMenu                 { background-color: #FFFFFF;   }"
                    "#MenuButton               { background-color: #FFFFFF;   color: #A8B0B9; font-size: 18px; }"
                    "#MenuButton:hover         { background-color: #F8A145; }"
                    "#MenuButton:checked       { background-color: #FFFFFF;   color: #F8A145}"
                    "#MenuButton:checked:hover { background-color: #FFFFFF; }"
                    "#MainMenuButton           { background-color: #FFFFFF;   border: none; } "
                    "#MainMenuButton:hover     { background-color: #FFFFFF;    } "
                    );

    w.show();
    return a.exec();
}
