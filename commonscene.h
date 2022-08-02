#ifndef COMMONSCENE_H
#define COMMONSCENE_H

#include "mainscene.h"
#include <QObject>

class CommonScene : public MainScene
{
    Q_OBJECT
public:
    CommonScene(QWidget *parent = nullptr);
};

#endif // COMMONSCENE_H
