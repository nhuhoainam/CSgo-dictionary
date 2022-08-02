#ifndef WORDVIEWER_H
#define WORDVIEWER_H

#include <QWidget>
#include <QIcon>

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

public slots:
    void setWord(const QString &key, std::vector<QString> defs, bool favState);

private:
    Ui::WordViewer *ui;
    QIcon on_icon;
    QIcon off_icon;
    bool hovered = false;
    bool checked = true;
signals:
    void wordRequest();
    void searchRequest(const QString&);
    void deleteRequest(const QString&);
    void editRequest(const QString&);
    void favoriteToggle(const QString&, bool checked);
};

#endif // WORDVIEWER_H
