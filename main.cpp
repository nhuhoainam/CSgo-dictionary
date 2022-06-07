#include "widget.h"

#include <QApplication>
#include <QLocale>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("CSgo Dictionary");
    w.show();
    return a.exec();
}
