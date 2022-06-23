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
signals:
    void searchRequest(const QString&);
    void deleteRequest(const QString&);
    void editRequest(const QString&);
    void favoriteToggle(const QString&, bool checked);
};

#endif // WORDVIEWER_H
