#include "dictionaryeditor.h"
#include "ui_dictionaryeditor.h"

DictionaryEditor::DictionaryEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DictionaryEditor)
{
    ui->setupUi(this);
    rmEditBtnGroup = new QButtonGroup;

    connect(ui->addBtn, &QPushButton::clicked, [=]() {
        auto key = ui->keywordEdit->text();
        ui->keywordEdit->setText("");
        std::vector<QString> defs;
        for (auto edit : defEditGroup) {
            if (edit.first->text().length() > 0) {
                defs.push_back(edit.first->text());
            }
        }
        for (auto edit : defEditGroup) {
            edit.first->setText("");
        }
        if (defs.size() > 0 && !key.isEmpty())
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
    QPushButton *newRemoveBtn = new QPushButton("-");
    newRemoveBtn->setFont(QFont("Sans", 16));
    newRemoveBtn->setMaximumWidth(48);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(newEdit);
    layout->addWidget(newRemoveBtn);
    layout->setContentsMargins(0,0,0,0);

    QWidget *container = new QWidget;
    container->setLayout(layout);
    defEditGroup.push_back(std::make_pair(newEdit, newRemoveBtn));
    ui->defLayout->addWidget(container);
    connect(newRemoveBtn, &QPushButton::clicked,
            this,
            [=]() {
        for (int i = 0; i < defEditGroup.size(); i++) {
            auto item = defEditGroup[i];
            if (item.second == newRemoveBtn) {
                removeEdit(i);
            }
        }
    });
}

void DictionaryEditor::resetDefEdit() {
    ui->keywordEdit->clear();
    QLayoutItem *child;
    while ((child = ui->defLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    defEditGroup.clear();
    addNewDefEdit();
}

void DictionaryEditor::removeEdit(int i) {
    QLayoutItem *item = ui->defLayout->takeAt(i);
    delete item->widget();
    delete item;
    defEditGroup.erase(defEditGroup.begin()+i);
}
