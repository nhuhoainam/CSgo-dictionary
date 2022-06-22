#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class SearchBox : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *textInp;
    QPushButton *searchBtn;
    QFrame *frame;
    QCompleter *completer;

    void setStyle();
public:
    explicit SearchBox(QWidget *parent = nullptr);

signals:
    void searchEdit(const QString &str);
    void searchFinished(const QString &str);

};

#endif // SEARCHBOX_H
