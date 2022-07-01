#include "home.h"
#include "ui_home.h"
#include "wordcard.h"
#include "searchbox.h"
#include "wordcardlayout.h"

#include <iostream>
#include <QListWidget>
#include <QKeyEvent>

char listViewStyle[] =
"QListView {"
"	border: 2px solid lightgrey;"
"	border-bottom-left-radius: 16px;"
"	border-bottom-right-radius: 16px;"
"	margin: 0;"
"	padding: 0 0 12px 0;"
"	background-color: white;"
"}"
""
"QListView::item {"
"	border-bottom: 2px solid lightgrey;"
"	padding: 0 0 0 12px;"
"}"
"QListView::item:alternate {"
"    background: #EEEEEE;"
"}"
""
"QListView::item:selected {"
"	background-color: lightgrey;"
"	color: black;"
"}"
""
"QListView::item:selected:active {"
"}"
""
"QListView::item:hover {"
"    background-color: lightgrey;"
"}";


Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    setupUi();
    connectSignalAndSlot();
}

void Home::setupUi() {
    ui->setupUi(this);

    for (int i = 0; i < 4; i++) {
        ui->wordGroup->addCard("heightist", "Someone who believes that people are superior to or inferior to others on account of their respective heights, or that people of differing heights have different moral qualities and intellectual capabilities.", true);
    }
    for (int i = 0; i < 4; i++) {
        ui->wordGroup->addCard("heightist", "Someone who believes that people are superior to or inferior to others on account of their respective heights, or that people of differing heights have different moral qualities and intellectual capabilities.", true);
    }
    completePopup = new QListWidget(this);
    completePopup->addItem("Hello");
    completePopup->addItem("Helo");
    completePopup->addItem("Hllo");
    completePopup->addItem("Helo");
    completePopup->addItem("ello");
    completePopup->hide();
    completePopup->setStyleSheet(listViewStyle);
}

void Home::connectSignalAndSlot() {
    connect(ui->searchBox, &SearchBox::searchFinished,
            this, &Home::searchRequest);
    connect(ui->searchBox, &SearchBox::searchEdit,
            this, &Home::completionRequest);
    connect(ui->wordGroup, &WordCardGroup::wordSelected,
            this, &Home::wordSelected);
    connect(ui->wordGroup, &WordCardGroup::wordToggleFavorite,
            this, &Home::wordToggleFavorite);
    connect(ui->resetBtn, &QPushButton::clicked,
            this, &Home::refreshRequest);
    connect(ui->searchBox, &SearchBox::requestCompletion, this, [=](const QString &str) {
        completePopup->show();
        ui->searchBox->handleCompleterShown();
    });
    connect(completePopup, &QListWidget::itemPressed, this, [=](QListWidgetItem *item) {
        ui->searchBox->handleCompletion(item->text());
        emit searchRequest(item->text());
        completePopup->setCurrentRow(-1);
        completePopup->hide();
    });
}

Home::~Home()
{
    delete ui;
}

void Home::setWordList(vector<tuple<QString, QString, bool>> words) {
    ui->wordGroup->clear();
    for (auto item : words) {
        auto keyword = std::get<0>(item);
        auto meaning = std::get<1>(item);
        auto favorite = std::get<2>(item);
        ui->wordGroup->addCard(keyword, meaning, favorite);
    }
}

void Home::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    completePopup->hide();
    ui->searchBox->handleCompleterHidden();
}
void Home::adjustPopup() {
    QRect rect = ui->searchBox->geometry();
    QPoint bottomLeft = mapToParent(rect.bottomLeft());
    completePopup->setGeometry(QRect(bottomLeft, QSize(rect.width(), 300)));
}

void Home::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    adjustPopup();
}

void Home::moveEvent(QMoveEvent *event) {
    QWidget::moveEvent(event);
    adjustPopup();
}

void Home::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up) {
        if (completePopup->isVisible()) {
            int cur = completePopup->currentRow();
            qDebug() << cur;
            if (cur < completePopup->count())
                completePopup->setCurrentRow(cur+1, QItemSelectionModel::Current);
        }
    } else if (event->key() == Qt::Key_Down) {
        qDebug() << "HERE";
        int cur = completePopup->currentRow();
        if (cur >= 0)
            completePopup->setCurrentRow(cur-1);
    }
}
