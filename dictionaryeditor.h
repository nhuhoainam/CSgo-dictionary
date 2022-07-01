#ifndef DICTIONARYEDITOR_H
#define DICTIONARYEDITOR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>

namespace Ui {
class DictionaryEditor;
}

class DictionaryEditor : public QWidget
{
    Q_OBJECT

public:
    explicit DictionaryEditor(QWidget *parent = nullptr);
    ~DictionaryEditor();
    void resetDefEdit();

private:
    Ui::DictionaryEditor *ui;
    std::vector<std::pair<QLineEdit*, QPushButton*>> defEditGroup;
    QButtonGroup *rmEditBtnGroup;

    void addNewDefEdit();
    void removeEdit(int i);
  signals:
    void addNewWord(const QString&, std::vector<QString>);
    void resetDictionary();
};

#endif // DICTIONARYEDITOR_H
