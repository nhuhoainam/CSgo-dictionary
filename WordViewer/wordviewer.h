#ifndef WORDVIEWER_H
#define WORDVIEWER_H

#include <QWidget>
#include <QIcon>
#include <string>
#include <QTextEdit>
#include "api/Word.h"

using std::string;
using std::pair;
class WordEdit;

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
    void setWord(const string &key, std::vector<pair<string, string>> defs, bool favState);
    void setWord(Word w);
    vector<WordEdit *> editGroup;

private:
    Ui::WordViewer *ui;
    QIcon on_icon;
    QIcon off_icon;
    bool hovered = false;
    bool checked = true;
    void clearEdits();
    void clearDisplayEntries();
    void removeEdit(int i);
    void handleDoneBtnPressed();
    void addEditEntry(const string def = "", const string example = "");
signals:
    void wordEdited(Word);
    void searchRequest(const QString&);
    void deleteRequest(const QString&);
    void favoriteToggle(const QString&, bool checked);
};

#endif // WORDVIEWER_H
