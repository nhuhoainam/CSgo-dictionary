#include "singlewordview.h"
#include "ui_singlewordview.h"

SingleWordView::SingleWordView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleWordView)
{
    ui->setupUi(this);

    completePopup = new QListWidget(this);
    completePopup->hide();

    connect(ui->widget,
            &WordViewer::deleteRequest,
            this,
            &SingleWordView::deleteRequest);
    connect(ui->widget,
            &WordViewer::favoriteToggle,
            this,
            &SingleWordView::favoriteToggle);
    connect(ui->widget,
            &WordViewer::wordEdited,
            this,
            &SingleWordView::wordEdited);
    connect(ui->searchBox,
            &SearchBox::requestCompletion,
            this,
            [=](const QString &str) {
        completePopup->show();
        ui->searchBox->handleCompleterShown();
    });
    connect(ui->searchBox,
            &SearchBox::searchFinished,
            this,
            &SingleWordView::handleSearch);
    connect(ui->searchBox,
            &SearchBox::searchEdit,
            this,
            &SingleWordView::completionRequest);
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
}

SingleWordView::~SingleWordView()
{
    delete ui;
}

void SingleWordView::setCompletionChoices(vector<QString> choices) {
    completePopup->clear();
    for (const auto &item : choices) {
        completePopup->addItem(item);
    }
}

void SingleWordView::setWord(Word w) {
    ui->widget->setWord(w);
}

void SingleWordView::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    completePopup->hide();
    ui->searchBox->handleCompleterHidden();
}
void SingleWordView::adjustPopup() {
    QRect rect = ui->searchBox->geometry();
    QPoint bottomLeft = mapToParent(rect.bottomLeft());
    completePopup->setGeometry(QRect(bottomLeft, QSize(rect.width(), 300)));
}

void SingleWordView::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    adjustPopup();
}

void SingleWordView::moveEvent(QMoveEvent *event) {
    QWidget::moveEvent(event);
    adjustPopup();
}

void SingleWordView::keyPressEvent(QKeyEvent *event) {
}

void SingleWordView::handleSearch(const QString &text) {
    ui->searchBox->handleCompletion(text);
    emit searchRequest(text);
    completePopup->setCurrentRow(-1);
    completePopup->hide();
}
