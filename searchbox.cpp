#include "searchbox.h"

#include <QHBoxLayout>

SearchBox::SearchBox(QWidget *parent)
    : QWidget{parent}
{
    textInp = new QLineEdit(this);
    textInp->setPlaceholderText("Type here to search...");
    textInp->setObjectName("TextInput");
    textInp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    searchBtn = new QPushButton(this);
    searchBtn->setObjectName("SearchButton");
    searchBtn->setIcon(QIcon("search.svg"));
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
    layout->setContentsMargins(20, 0, 7, 0);
    setFixedHeight(40);

    connect(searchBtn, &QPushButton::clicked, this, [=]() {
        emit searchFinished(textInp->text());
    });

    connect(textInp, &QLineEdit::returnPressed, this, [=]() {
        emit searchFinished(textInp->text());
    });

    connect(textInp, &QLineEdit::textChanged, this, [=](const QString &str) {
        emit searchEdit(str);
    });
}

void SearchBox::setStyle() {
    QString backgroundColor = "white";
    auto frameStyleSheet = QString("border-radius: 20px; border: 1px solid grey; background-color: white;");
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
