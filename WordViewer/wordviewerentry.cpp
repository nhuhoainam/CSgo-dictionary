#include "wordviewerentry.h"
#include "ui_wordviewerentry.h"

WordViewerEntry::WordViewerEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordViewerEntry)
{
    ui->setupUi(this);
}

WordViewerEntry::~WordViewerEntry()
{
    delete ui;
}

WordViewerEntry::WordViewerEntry(const string &def, const string &example, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::WordViewerEntry)
{
    ui->setupUi(this);

    ui->defLb->setText(QString::fromStdString(def));
    string tmp = example;
    if (example == "")
        ui->exampleWidget->hide();
    else {
        size_t index = tmp.find('+');
        if(index != std::string::npos)
            tmp[index] = '\n';
        ui->exampleLb->setText(QString::fromStdString(tmp));
    }
}
