#ifndef WORDVIEWER_H
#define WORDVIEWER_H

#include <QWidget>

namespace Ui {
class WordViewer;
}

class WordViewer : public QWidget
{
    Q_OBJECT

public:
    explicit WordViewer(QWidget *parent = nullptr);
    ~WordViewer();

    void paintEvent(QPaintEvent*) override;

private:
    Ui::WordViewer *ui;
};

#endif // WORDVIEWER_H
