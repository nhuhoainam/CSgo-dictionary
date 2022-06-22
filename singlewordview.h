#ifndef SINGLEWORDVIEW_H
#define SINGLEWORDVIEW_H

#include <QWidget>

namespace Ui {
class SingleWordView;
}

class SingleWordView : public QWidget
{
    Q_OBJECT

public:
    explicit SingleWordView(QWidget *parent = nullptr);
    ~SingleWordView();

private:
    Ui::SingleWordView *ui;
};

#endif // SINGLEWORDVIEW_H
