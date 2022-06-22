#include "sidebar.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QEvent>

#define action_height 40


Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent)
    , mCheckedAction(NULL)
    , mOverAction(NULL)
    , mSidebarWidth(200)
    , mIconSize(QSize(24, 24))
    , mSidebarIcon(new QIcon())
{
    setFixedWidth(200);
    setMouseTracking(true);
}

void Sidebar::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    int action_y = 100;
    p.fillRect(rect(), QColor(255, 255, 255));
    QRect iconRect(0, 0, mSidebarWidth, 100);
//    QRect centerRect(0, 0, 150, 100);
//    centerRect.moveCenter(iconRect.center());
    mSidebarIcon->paint(&p, iconRect);
    for(auto action: mActions)
    {

        QRect actionRect(0, action_y, event->rect().width(), action_height);

        if(action->isChecked())
        {
            p.fillRect(actionRect, QColor(255, 248, 239));
        }

        if(action == mOverAction){
            p.fillRect(actionRect, QColor(255, 248, 239));
        }

        p.setPen(QColor(168, 176, 185));
        QRect iconRect(24, action_y + action_height/2 - 12, mIconSize.width(), mIconSize.height());
        QIcon actionIcon = action->icon();
        actionIcon.paint(&p, iconRect);
        QSize size = p.fontMetrics().size(Qt::TextSingleLine, action->text());
        QRect textRect(
                    24 + mIconSize.width() + 20,
                    actionRect.bottom() - size.height()/2 - (action_height/2),
                    mSidebarWidth - size.width() - 24,
                    action_height
                    );
        p.drawText(textRect, Qt::AlignLeft, action->text());
        action_y += actionRect.height();
    }
}

QSize Sidebar::minimumSizeHint() const
{
    return action_height*QSize(1, mActions.size());
}

void Sidebar::addAction(QAction *action)
{
    mActions.push_back(action);
    action->setCheckable(true);
    update();
}

QAction *Sidebar::addAction(const QString &text, const QIcon &icon)
{
    QAction *action = new QAction(icon, text, this);
    action->setCheckable(true);
    mActions.push_back(action);
    update();
    return action;
}

void Sidebar::mousePressEvent(QMouseEvent *event)
{
    QAction* tempAction = actionAt(event->pos());
    if(tempAction == NULL || tempAction->isChecked())
        return;
    qDebug()<<"clicked";
    if(mCheckedAction)
        mCheckedAction->setChecked(false);
    if(mOverAction == tempAction)
        mOverAction = NULL;
    mCheckedAction = tempAction;
    tempAction->setChecked(true);
    update();
    QWidget::mousePressEvent(event);
}


void Sidebar::mouseMoveEvent(QMouseEvent *event)
{
    QAction* tempAction = actionAt(event->pos());
    if(tempAction == NULL){
        mOverAction = NULL;
        update();
        return;
    }
    if(tempAction->isChecked() || mOverAction == tempAction)
        return;
    mOverAction = tempAction;
    update();
    QWidget::mouseMoveEvent(event);
}

void Sidebar::leaveEvent(QEvent * event)
{
    mOverAction = NULL;
    update();
    QWidget::leaveEvent(event);
}

QAction* Sidebar::actionAt(const QPoint &at)
{
    int action_y = 100;
    for(auto action: mActions)
    {
        QRect actionRect(0, action_y, rect().width(), action_height);
        if(actionRect.contains(at))
            return action;
        action_y += actionRect.height();
    }
    return NULL;
}

void Sidebar::setSidebarIcon(QIcon &icon) {
    mSidebarIcon = &icon;
}

#undef action_height
