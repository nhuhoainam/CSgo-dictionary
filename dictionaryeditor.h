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
    std::vector<std::pair<QLineEdit*, QPushButton*>> exampleEditGroup;
    QButtonGroup *rmEditBtnGroup;

    void addNewDefEdit();
    void addNewExEdit();
    void removeEdit(int i);
  signals:
    void addNewWord(const QString&, std::vector<std::pair<QString, QString> >);
    void resetDictionary();
};

#endif // DICTIONARYEDITOR_H
