#ifndef DICTIONARYEDITOR_H
#define DICTIONARYEDITOR_H

#include <QWidget>

namespace Ui {
class DictionaryEditor;
}

class DictionaryEditor : public QWidget
{
    Q_OBJECT

public:
    explicit DictionaryEditor(QWidget *parent = nullptr);
    ~DictionaryEditor();

private:
    Ui::DictionaryEditor *ui;
};

#endif // DICTIONARYEDITOR_H
