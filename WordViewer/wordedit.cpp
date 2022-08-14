#include "wordedit.h"
#include "ui_wordedit.h"

WordEdit::WordEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordEdit)
{
    ui->setupUi(this);
}

WordEdit::~WordEdit()
{
    delete ui;
}

WordEdit::WordEdit(const string &def, const string &example, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::WordEdit)
{
    ui->setupUi(this);

    ui->defEdit->setText(QString::fromStdString(def));
    ui->exampleEdit->setText(QString::fromStdString(example));
    connect(
                ui->deleteBtn,
                &QPushButton::clicked,
                this,
                &WordEdit::wordEditDelete);
}

std::pair<string, string> WordEdit::definition() const {
    return {
        ui->defEdit->toPlainText().toStdString(),
        ui->exampleEdit->toPlainText().toStdString()
    };
}
