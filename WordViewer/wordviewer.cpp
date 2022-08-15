#include "wordviewer.h"
#include "ui_wordviewer.h"
#include "WordViewer/wordviewerentry.h"
#include "WordViewer/wordedit.h"

#include <QPainter>
#include <QTextEdit>

WordViewer::WordViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordViewer)
{
    ui->setupUi(this);

    connect(ui->deleteBtn,
            &QPushButton::clicked,
            this,
            [=]() {
        auto keyword = ui->keyword->text();
        emit deleteRequest(keyword);
    });
    connect(ui->editBtn,
            &QPushButton::clicked,
            this,
            [=]() {
        auto keyword = ui->keyword->text();
        ui->modeStack->setCurrentIndex(1);
        ui->editBtn->hide();
    });
    connect(ui->doneEditBtn,
            &QPushButton::clicked,
            this,
            &WordViewer::handleDoneBtnPressed);
    connect(ui->favBtn,
            &QPushButton::clicked,
            this,
            [=](bool state) {
        auto keyword = ui->keyword->text();
        emit favoriteToggle(keyword, state);
    });
    connect(ui->addEntryBtn,
            &QPushButton::clicked,
            this,
            [=]() {
        addEditEntry();
    });
}

WordViewer::~WordViewer()
{
    delete ui;
}

void WordViewer::clearEdits() {
    QLayoutItem *item;
    while ((item = ui->defEditLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
    editGroup.clear();
}

void WordViewer::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);


    QRect boundaryRect = rect();
    boundaryRect.adjust(painter.pen().width(),
                        painter.pen().width(),
                        -painter.pen().width(),
                        -painter.pen().width());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(boundaryRect, 10.0, 10.0);
    QWidget::paintEvent(event);
}

void WordViewer::setWord(Word w) {
    clearEdits();
    clearDisplayEntries();
    ui->keyword->setText(QString::fromStdString(w.word));
    for (auto &p : w.data) {
        auto *w = new WordViewerEntry(p.first, p.second, this);
        ui->defLayout->addWidget(w);
        addEditEntry(p.first, p.second);
    }
}

void WordViewer::removeEdit(int i) {
    QLayoutItem *item = ui->defEditLayout->takeAt(i);
    delete item->widget();
    delete item;
    editGroup.erase(editGroup.begin()+i);
}

void WordViewer::handleDoneBtnPressed() {
    Word w(ui->keyword->text().toStdString());
    for (auto widget : editGroup) {
        auto def = widget->definition();
        w.data.push_back({def.first, def.second});
    }
    update();
    ui->editBtn->show();
    ui->modeStack->setCurrentIndex(0);

    emit wordEdited(w);
}

void WordViewer::addEditEntry(const string def, const string example) {
    auto *edit = new WordEdit(def, example);
    ui->defEditLayout->addWidget(edit);
    editGroup.push_back(edit);
    connect(edit,
        &WordEdit::wordEditDelete,
        this,
        [=]() {
        for (int i = 0; i < editGroup.size(); i++) {
            if (editGroup[i] == edit) {
                removeEdit(i);
            }
        }
    });
}

void WordViewer::clearDisplayEntries() {
    QLayoutItem *item;
    while ((item = ui->defLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
}
