#include "wordcard.h"
#include "ui_wordcard.h"

WordCard::WordCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordCard)
{
    effect = new QGraphicsDropShadowEffect(this);
    on_icon = QIcon(":/images/img/bookmark_solid.svg");
    off_icon = QIcon(":/images/img/bookmark_hollow.svg");
    setupUi();
    connectSignalAndSlot();
}

void WordCard::connectSignalAndSlot() {
    connect(ui->favBtn, &QPushButton::clicked, this, [=](bool checked) {
        emit favoriteStateChanged(ui->keyword->text(), checked);
        if (checked)
            ui->favBtn->setIcon(on_icon);
        else
            ui->favBtn->setIcon(off_icon);
    });
}

void WordCard::setupUi() {
    ui->setupUi(this);
    effect->setColor(Qt::gray);
    effect->setOffset(3,3);
    effect->setBlurRadius(10);
    effect->setEnabled(false);
    setGraphicsEffect(effect);
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);
    ui->favBtn->setIcon(off_icon);
}

void WordCard::setFavoriteState(bool on) {
    ui->favBtn->setChecked(on);
    if (on) {
        ui->favBtn->setIcon(on_icon);
    } else {
        ui->favBtn->setIcon(off_icon);
    }
}

WordCard::~WordCard()
{
    delete ui;
}

bool WordCard::event(QEvent* event) {
    if (event->type() == QEvent::HoverEnter) {
        hovered = true;
        effect->setEnabled(true);
        update();
        return true;
    } else if (event->type() == QEvent::HoverLeave) {
        hovered = false;
        effect->setEnabled(false);
        update();
        return true;
    }

    return QWidget::event(event);
}

void WordCard::mousePressEvent(QMouseEvent*event) {
    update();
    emit wordSelected(ui->keyword->text());
}

void WordCard::mouseReleaseEvent(QMouseEvent*) {
    update();
}

void WordCard::setKeyword(const QString &keyword) {
    ui->keyword->setText(keyword);
}

void WordCard::addDefinition(const QString &def) {
    ui->defLayout->addWidget(new QLabel(def));
}
