#include "searchbox.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QKeyEvent>

SearchBox::SearchBox(QWidget *parent)
    : QWidget{parent}
{
    setMinimumWidth(350);
    auto sizePolicy = QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    setSizePolicy(sizePolicy);
    textInp = new QLineEdit(this);
    textInp->setPlaceholderText("Type here to search...");
    textInp->setObjectName("TextInput");
    textInp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    searchBtn = new QPushButton(this);
    searchBtn->setObjectName("SearchButton");
    searchBtn->setIcon(QIcon(":/images/img/search.svg"));
    searchBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    frame = new QFrame(this);
    frame->setObjectName("Frame");
    setStyle();

    auto *vlayout = new QVBoxLayout(this);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->addWidget(frame);

    auto *layout = new QHBoxLayout(frame);
    layout->addWidget(textInp);
    layout->addWidget(searchBtn);
    layout->setSpacing(0);
    layout->setContentsMargins(16, 0, 7, 0);
    setFixedHeight(32);

    connect(searchBtn, &QPushButton::clicked, this, [=]() {
        emit searchFinished(textInp->text());
    });

    connect(textInp, &QLineEdit::returnPressed, this, [=]() {
        emit searchFinished(textInp->text());
    });

    connect(textInp, &QLineEdit::textChanged, this, [=](const QString &str) {
        emit searchEdit(str);
        if (str.length() >= 2) {
            emit requestCompletion(str);
        }
    });
}

void SearchBox::moveEvent(QMoveEvent *event) {
    QWidget::moveEvent(event);
}

void SearchBox::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}

void SearchBox::setStyle() {
    QString backgroundColor = "white";
    auto frameStyleSheet = QString("border-radius: 16px;"
                                   "border: 2px solid lightgrey;"
                                   "background-color: white;");
    auto textInpStyleSheet = QString("border: none;"
                                     "background-color: %5;")
            .arg(backgroundColor);
    auto searchBoxStyleSheet = QString("border: none;"
                                       "background-color: %5;")
            .arg(backgroundColor);
    textInp->setStyleSheet(textInpStyleSheet);
    searchBtn->setStyleSheet(searchBoxStyleSheet);
    frame->setStyleSheet(frameStyleSheet);
}

void SearchBox::setWithCompleterStyle() {
    QString backgroundColor = "white";
    auto frameStyleSheet = QString("border-top-left-radius: 16px;"
                                   "border-top-right-radius: 16px;"
                                   "border: 2px solid lightgrey;"
                                   "background-color: white;");
    auto textInpStyleSheet = QString("border: none;"
                                     "background-color: %5;")
            .arg(backgroundColor);
    auto searchBoxStyleSheet = QString("border: none;"
                                       "background-color: %5;")
            .arg(backgroundColor);
    textInp->setStyleSheet(textInpStyleSheet);
    searchBtn->setStyleSheet(searchBoxStyleSheet);
    frame->setStyleSheet(frameStyleSheet);
}

void SearchBox::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    if (event->key() == Qt::Key_Up) {
        emit requestPrevCompletion();
    } else if (event->key() == Qt::Key_Down) {
        emit requestNextCompletion();
    }
}

void SearchBox::handleCompleterShown() {
    setWithCompleterStyle();
}

void SearchBox::handleCompleterHidden() {
    setStyle();
}

void SearchBox::handleCompletion(const QString &str) {
    textInp->setText("");
    setStyle();
}

void SearchBox::setText(const QString &str) {
    textInp->setText(str);
}

QString SearchBox::text() const {
    return textInp->text();
}
