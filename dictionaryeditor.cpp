#include "dictionaryeditor.h"
#include "ui_dictionaryeditor.h"

DictionaryEditor::DictionaryEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DictionaryEditor)
{
    ui->setupUi(this);

    connect(ui->addBtn, &QPushButton::clicked, [=]() {
        auto key = ui->keywordEdit->text();
        ui->keywordEdit->setText("");
        std::vector<QString> defs;
        for (auto edit : defEditGroup) {
            if (edit->text().length() > 0) {
                defs.push_back(edit->text());
            }
        }
        for (auto edit : defEditGroup) {
            edit->setText("");
        }
        emit addNewWord(key, defs);
    });
    connect(ui->resetBtn,
            &QPushButton::clicked,
            this,
            &DictionaryEditor::resetDictionary);
    connect(ui->addDefBtn, &QPushButton::clicked,
            this, &DictionaryEditor::addNewDefEdit);
    addNewDefEdit();
}

DictionaryEditor::~DictionaryEditor()
{
    delete ui;
}

void DictionaryEditor::addNewDefEdit() {
    QLineEdit *newEdit = new QLineEdit;
    defEditGroup.push_back(newEdit);
    ui->defLayout->addWidget(newEdit);
}
