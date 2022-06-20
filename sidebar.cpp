#include "sidebar.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QActionGroup>
#include <QString>
#include <QStyleOption>
#include <QPainter>
#include <QPropertyAnimation>

static const QString MenuButtonObjectName("MenuButton");
static const QString MainMenuName("MainMenu");
static const QString MainMenuButtonObjectName("MainMenuButton");

class MenuButton : public QPushButton
{
public:
    MenuButton(QAction* action, QWidget* parent)
        : QPushButton(parent)
        , mIconSize(QSize(64,64))
        , mAction(action)
    {
        setObjectName(MenuButtonObjectName);
        connect(action, &QAction::destroyed, this, &MenuButton::deleteLater);
        setCursor(Qt::PointingHandCursor);

        connect(mAction, SIGNAL(changed()), this, SLOT(update()));
        connect(this, &MenuButton::clicked, [&]{
            if(mAction->isCheckable() && !mAction->isChecked())
                mAction->toggle();

            mAction->trigger();
        });
    }

    void paintEvent(QPaintEvent*) override
    {
        QPainter painter(this);
        QStyleOptionButton opt;
        opt.initFrom(this);
        opt.state |= (mAction->isChecked() ? QStyle::State_On : QStyle::State_Off);

        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        const QRect contentsRect = style()->subElementRect(QStyle::SE_PushButtonContents, &opt, this);
        if(!mAction->icon().isNull())
        {
            QIcon::Mode mode = ((opt.state & QStyle::State_MouseOver) == 0) ? QIcon::Normal : QIcon::Active;
            if(!isEnabled())
                mode = QIcon::Disabled;
            QIcon::State state = mAction->isChecked() ? QIcon::On : QIcon::Off;
            painter.drawPixmap(QRect(contentsRect.topLeft(), mIconSize), mAction->icon().pixmap(mIconSize, mode, state));
        }

        opt.rect = contentsRect.adjusted(mIconSize.width()+1, 0, 0, 0);
        opt.text = fontMetrics().elidedText(mAction->iconText(), Qt::ElideRight, opt.rect.width(), Qt::TextShowMnemonic);
        style()->drawControl(QStyle::CE_CheckBoxLabel, &opt, &painter, this);
    }

    void setIconSize(const QSize& size) { mIconSize = size; setFixedHeight(mIconSize.height() * 2); update(); }

    QAction* action() const { return mAction; }

private:
    QSize mIconSize;
    QAction* mAction;
};

Sidebar::Sidebar(QWidget* parent)
    : QWidget(parent)
    , mActions(new QActionGroup(this))
    , mMenuButton(new QPushButton(this))
    , mMenuWidth(200)
    , mAnimated(true)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    mMenuButton->setObjectName(MainMenuButtonObjectName);
    mMenuButton->setFlat(true);
    mMenuButton->setIconSize(QSize(48,48));
    mMenuButton->setFixedSize(48,48);
    mMenuButton->setCheckable(true);
    mMenuButton->setCursor(Qt::PointingHandCursor);
    mActions->setExclusive(true);

    auto menuWidget = new QWidget(this);
    menuWidget->setObjectName(MainMenuName);
    auto menuLay = new QHBoxLayout();
    menuLay->setContentsMargins(0, 0, 0, 0);
    menuLay->setSpacing(0);
    menuLay->addWidget(mMenuButton);
    menuLay->addWidget(menuWidget);

    auto lay = new QVBoxLayout();
    setLayout(lay);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);

    lay->addLayout(menuLay);
    lay->addStretch();
    setFixedWidth(48);

    connect(mMenuButton, &QPushButton::toggled, this, &Sidebar::setExpansionState);
    connect(mMenuButton, &QPushButton::toggled, this, &Sidebar::expandedChanged);
    connect(mActions, &QActionGroup::triggered, this, &Sidebar::triggered);
}

QIcon Sidebar::menuIcon() const
{
    return mMenuButton->icon();
}

QSize Sidebar::iconSize() const
{
    return mMenuButton->iconSize();
}

int Sidebar::menuWidth() const
{
    return mMenuWidth;
}

QList<QAction*> Sidebar::actions() const
{
    return mActions->actions();
}

QAction* Sidebar::addMenuAction(QAction* action)
{
    mActions->addAction(action);
    registerAction(action);
    return action;
}

QAction*Sidebar::addMenuAction(const QString& label)
{
    auto action = mActions->addAction(label);
    action->setCheckable(true);
    registerAction(action);
    return action;
}

QAction*Sidebar::addMenuAction(const QIcon& icon, const QString& label)
{
    auto action = mActions->addAction(icon, label);
    action->setCheckable(true);
    registerAction(action);
    return action;
}

void Sidebar::removeMenuAction(QAction* action)
{
    mActions->removeAction(action);
    unRegisterAction(action);
}

void Sidebar::setMenuIcon(const QIcon& icon)
{
    mMenuButton->setIcon(icon);
    emit iconChanged();
}

void Sidebar::setIconSize(const QSize& size)
{
    if(size == mMenuButton->iconSize())
        return;

    mMenuButton->setIconSize(size);
    mMenuButton->setFixedSize(size);
    auto buttons = findChildren<MenuButton*>(MenuButtonObjectName);
    for(auto btn : buttons)
        btn->setIconSize(size);

    if(mMenuButton->isChecked())
        setFixedWidth(mMenuButton->width() + mMenuWidth);
    else
        setFixedWidth(mMenuButton->width());

    emit iconSizeChanged(size);
}

void Sidebar::setMenuWidth(int width)
{
    if(width == mMenuWidth)
        return;

    mMenuWidth = width;

    if(mMenuButton->isChecked())
        setFixedWidth(mMenuButton->width() + mMenuWidth);
    else
        setFixedWidth(mMenuButton->width());

    emit menuWidthChanged(mMenuWidth);
}

void Sidebar::setExpansionState(bool expanded)
{
    if(mAnimated)
    {
        auto anim = new QPropertyAnimation(this, "minimumWidth", this);
        anim->setDuration(250);
        anim->setStartValue(mMenuButton->width());
        anim->setEndValue(mMenuButton->width() + mMenuWidth);
        anim->setDirection(expanded ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        anim->setEasingCurve(expanded ? QEasingCurve::OutCubic : QEasingCurve::InCubic);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        if(expanded)
            setFixedWidth(mMenuButton->width() + mMenuWidth);
        else
            setFixedWidth(mMenuButton->width());
    }
}

void Sidebar::registerAction(QAction* action)
{
    auto button = new MenuButton(action, this);
    button->setIconSize(mMenuButton->iconSize());
    auto lay = static_cast<QVBoxLayout*>(layout());
    lay->insertWidget(lay->count() - 1, button);
}

void Sidebar::unRegisterAction(QAction* action)
{
    auto buttons = findChildren<MenuButton*>(MenuButtonObjectName);
    auto btn = std::find_if(buttons.begin(), buttons.end(), [&](MenuButton* btn){ return btn->action() == action; });
    if(btn != buttons.end())
        delete *btn;
}

bool Sidebar::animated() const
{
    return mAnimated;
}

bool Sidebar::expanded() const
{
    return mMenuButton->isChecked();
}

void Sidebar::setAnimated(bool animated)
{
    if(mAnimated == animated)
        return;

    mAnimated = animated;
    emit animatedChanged(mAnimated);
}

void Sidebar::setExpanded(bool expanded)
{
    mMenuButton->setChecked(expanded);
}

void Sidebar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
