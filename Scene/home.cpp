#include "home.h"
#include "ui_mainscene.h"

#include <QPushButton>

Home::Home(QWidget *parent)
    : MainScene("Random new word", parent)
{
    setupRefreshBtn();
}

void Home::setupRefreshBtn() {
    refreshBtn = new QPushButton;
    ui->titleLayout->insertWidget(1, refreshBtn);
    connect(refreshBtn, &QPushButton::clicked,
            this, &Home::refreshRequest);
    refreshBtn->setStyleSheet("QPushButton {"
                              "border-radius: 6px;"
                              "border: none;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: lightgrey;"
                              "}");
    refreshBtn->setIcon(QIcon(":/images/img/reset.svg"));
    refreshBtn->setIconSize(QSize(24, 24));
}
