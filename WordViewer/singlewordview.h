#ifndef SINGLEWORDVIEW_H
#define SINGLEWORDVIEW_H

#include <QWidget>
#include <QListWidget>
#include "api/Word.h"

namespace Ui {
class SingleWordView;
}

class SingleWordView : public QWidget
{
    Q_OBJECT

public:
    explicit SingleWordView(QWidget *parent = nullptr);
    ~SingleWordView();
    void setWord(Word w);
    void setCompletionChoices(vector<QString>);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void changeDictionary(const QString &);

private:
    Ui::SingleWordView *ui;
    QListWidget *completePopup;
    void handleSearch(const QString &text);
    void adjustPopup();
signals:
    void wordEdited(Word);
    void completionRequest(const QString &);
    void searchRequest(const QString&);
    void deleteRequest(const QString&);
    void favoriteToggle(const QString&, bool checked);
};

#endif // SINGLEWORDVIEW_H
