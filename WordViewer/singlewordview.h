#ifndef SINGLEWORDVIEW_H
#define SINGLEWORDVIEW_H

#include <QWidget>
#include "api/dictionary_UI_testing.hpp"

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

private:
    Ui::SingleWordView *ui;
signals:
    void wordEdited(Word);
    void searchRequest(const QString&);
    void deleteRequest(const QString&);
    void favoriteToggle(const QString&, bool checked);
};

#endif // SINGLEWORDVIEW_H
