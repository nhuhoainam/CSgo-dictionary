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
        std::vector<std::pair<QString, QString> > defs;
        for (auto def = defEditGroup.begin(), example = exampleEditGroup.begin();
             def != defEditGroup.end() && example != exampleEditGroup.end();
             def++, example++)
        {
            if (def->first->text().length() > 0) {
                defs.push_back(std::pair(def->first->text(), example->first->text()));
            }
        }
        for (auto edit = defEditGroup.begin(), example = exampleEditGroup.begin();
             edit != defEditGroup.end() && example != exampleEditGroup.end();
             edit++, example++)
        {
            edit->first->setText("");
            example->first->setText("");
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
    connect(ui->addDefBtn, &QPushButton::clicked,
            this, &DictionaryEditor::addNewExEdit);
    addNewDefEdit();
    addNewExEdit();
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

void DictionaryEditor::addNewExEdit() {
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
    exampleEditGroup.push_back(std::make_pair(newEdit, newRemoveBtn));
    ui->exampleLayout->addWidget(container);
    connect(newRemoveBtn, &QPushButton::clicked,
            this,
            [=]() {
        for (int i = 0; i < exampleEditGroup.size(); i++) {
            auto item = exampleEditGroup[i];
            if (item.second == newRemoveBtn) {
                removeExample(i);
            }
        }
    });
}

void DictionaryEditor::resetDefEdit() {
    ui->keywordEdit->clear();
    QLayoutItem *def, *ex;
    while ((def = ui->defLayout->takeAt(0)) != nullptr
           && (ex = ui->exampleLayout->takeAt(0)) != nullptr)
    {
        delete def->widget();
        delete def;
        delete ex->widget();
        delete ex;
    }
    defEditGroup.clear();
    exampleEditGroup.clear();
    addNewDefEdit();
    addNewExEdit();
}

void DictionaryEditor::removeEdit(int i) {
    QLayoutItem *def = ui->defLayout->takeAt(i);
    delete def->widget();
    delete def;
    defEditGroup.erase(defEditGroup.begin()+i);
}

void DictionaryEditor::removeExample(int i) {
    QLayoutItem *ex = ui->exampleLayout->takeAt(i);
    delete ex->widget();
    delete ex;
    exampleEditGroup.erase(exampleEditGroup.begin()+i);
}
