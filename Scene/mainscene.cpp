#include "mainscene.h"
#include "ui_mainscene.h"
#include "api/Word.h"

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

MainScene::MainScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainScene)
{
    setupUi();
    connectSignalAndSlot();
}

MainScene::MainScene(QString title, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainScene)
{
    setupUi();
    ui->title->setText(title);
    connectSignalAndSlot();
}

void MainScene::setupUi() {
    ui->setupUi(this);

    completePopup = new QListWidget(this);
    completePopup->hide();
    completePopup->setStyleSheet(listViewStyle);
}

void MainScene::setCompletionChoices(vector<QString> choices) {
    completePopup->clear();
    for (const auto &item : choices) {
        completePopup->addItem(item);
    }
}
void MainScene::changeDictionary(const QString &dict) {
    ui->dictionaryBox->setCurrentText(dict);
}

void MainScene::setWordList(vector<pair<Word, bool>> words) {
    ui->wordGroup->clear();
    for (auto item : words) {
        auto word = std::get<0>(item);
        auto fav = std::get<1>(item);
        ui->wordGroup->addCard(word, fav);
    }
}

void MainScene::connectSignalAndSlot() {
    connect(ui->searchBox,
            &SearchBox::searchFinished,
            this,
            &MainScene::handleSearch);
    connect(ui->searchBox,
            &SearchBox::searchEdit,
            this,
            &MainScene::completionRequest);
    connect(ui->wordGroup,
            &WordCardGroup::wordSelected,
            this,
            &MainScene::wordSelected);
    connect(ui->wordGroup,
            &WordCardGroup::wordToggleFavorite,
            this,
            &MainScene::wordToggleFavorite);
    connect(ui->wordGroup,
            &WordCardGroup::wordFavorite,
            this,
            &MainScene::wordFavorite);
    connect(ui->wordGroup,
            &WordCardGroup::wordUnfavorite,
            this,
            &MainScene::wordUnfavorite);
    connect(ui->searchBox,
            &SearchBox::requestCompletion,
            this,
            [=](const QString &str) {
        completePopup->show();
        ui->searchBox->handleCompleterShown();
    });
    connect(completePopup,
            &QListWidget::itemPressed,
            this,
            [=](QListWidgetItem *item) {
        handleSearch(item->text());
    });
    connect(ui->searchBox,
            &SearchBox::requestNextCompletion,
            this,
            [=]() {
        int cur = completePopup->currentRow();
        cur += 1;
        completePopup->setCurrentRow(cur);
        auto curItem = completePopup->item(cur);
        if (curItem)
            ui->searchBox->setText(curItem->text());
    });
    connect(ui->searchBox,
            &SearchBox::requestPrevCompletion,
            this,
            [=]() {
        int cur = completePopup->currentRow();
        cur -= 1;
        completePopup->setCurrentRow(cur);
        auto curItem = completePopup->item(cur);
        if (curItem)
            ui->searchBox->setText(curItem->text());
    });
    connect(ui->dictionaryBox,
            &QComboBox::currentTextChanged,
            this,
            &MainScene::dictionaryTypeChange);
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    completePopup->hide();
    ui->searchBox->handleCompleterHidden();
}
void MainScene::adjustPopup() {
    QRect rect = ui->searchBox->geometry();
    QPoint bottomLeft = mapToParent(rect.bottomLeft());
    completePopup->setGeometry(QRect(bottomLeft, QSize(rect.width(), 300)));
}

void MainScene::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    adjustPopup();
}

void MainScene::moveEvent(QMoveEvent *event) {
    QWidget::moveEvent(event);
    adjustPopup();
}

void MainScene::keyPressEvent(QKeyEvent *event) {
}

void MainScene::handleSearch(const QString &text) {
    ui->searchBox->handleCompletion(text);
    emit searchRequest(text);
    completePopup->setCurrentRow(-1);
    completePopup->hide();
}
