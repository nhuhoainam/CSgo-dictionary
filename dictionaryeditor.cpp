#include "dictionaryeditor.h"
#include "ui_dictionaryeditor.h"

DictionaryEditor::DictionaryEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DictionaryEditor)
{
    ui->setupUi(this);

    connect(ui->addBtn, &QPushButton::clicked, [=]() {
        auto key = ui->keywordEdit->text();
        auto def = ui->definitionEdit->text();
        emit addNewWord(key, def);
    });
    connect(ui->resetBtn,
            &QPushButton::clicked,
            this,
            &DictionaryEditor::resetDictionary);
}

DictionaryEditor::~DictionaryEditor()
{
    delete ui;
}
