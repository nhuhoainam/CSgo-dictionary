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
    ui->exampleLb->setText(QString::fromStdString(example));
}
