#include "dictionaryeditor.h"
#include "ui_dictionaryeditor.h"

DictionaryEditor::DictionaryEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DictionaryEditor)
{
    ui->setupUi(this);
}

DictionaryEditor::~DictionaryEditor()
{
    delete ui;
}
