#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QAbstractItemView>

class SearchBox : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *textInp;
    QPushButton *searchBtn;
    QFrame *frame;

    void setStyle();
    void connectSignalAndSlot();
    void setupUI();
    void setWithCompleterStyle();
    void adjustCompleter();
public:
    explicit SearchBox(QWidget *parent = nullptr);
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void handleCompleterShown();
    void handleCompleterHidden();

public slots:
    void handleCompletion(const QString &str);

signals:
    void searchEdit(const QString &str);
    void searchFinished(const QString &str);
    void requestCompletion(const QString &str);
};

#endif // SEARCHBOX_H
