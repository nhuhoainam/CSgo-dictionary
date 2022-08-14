#ifndef WORDEDIT_H
#define WORDEDIT_H

#include <QWidget>
#include <string>
using std::string;

namespace Ui {
class WordEdit;
}

class WordEdit : public QWidget
{
    Q_OBJECT

public:
    explicit WordEdit(QWidget *parent = nullptr);
    WordEdit(const string &def, const string &example, QWidget *parent = nullptr);
    std::pair<string, string> definition() const;
    ~WordEdit();

private:
    Ui::WordEdit *ui;
signals:
    void wordEditDelete();
};

#endif // WORDEDIT_H
