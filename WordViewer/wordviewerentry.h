#ifndef WORDVIEWERENTRY_H
#define WORDVIEWERENTRY_H

#include <QWidget>
#include <string>
using std::string;

namespace Ui {
class WordViewerEntry;
}

class WordViewerEntry : public QWidget
{
    Q_OBJECT

public:
    explicit WordViewerEntry(QWidget *parent = nullptr);
    WordViewerEntry(const string &, const string &, QWidget *parent = nullptr);
    ~WordViewerEntry();

private:
    Ui::WordViewerEntry *ui;
};

#endif // WORDVIEWERENTRY_H
