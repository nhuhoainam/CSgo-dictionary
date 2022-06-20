#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QList>
#include <QSize>
#include <QIcon>

class QPushButton;
class QActionGroup;
class QAction;
class QString;

class Sidebar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QIcon icon      READ menuIcon WRITE setMenuIcon NOTIFY iconChanged)
    Q_PROPERTY(QSize iconSize  READ iconSize   WRITE setIconSize   NOTIFY iconSizeChanged)
    Q_PROPERTY(int   menuWidth READ menuWidth  WRITE setMenuWidth  NOTIFY menuWidthChanged)
    Q_PROPERTY(bool  animated  READ animated   WRITE setAnimated   NOTIFY animatedChanged)
    Q_PROPERTY(bool  expanded  READ expanded   WRITE setExpanded   NOTIFY expandedChanged)
public:
    explicit Sidebar(QWidget *parent = nullptr);

    QIcon menuIcon() const;
    QSize iconSize() const;
    int menuWidth() const;
    QList<QAction*>	actions() const;
    bool animated() const;
    bool expanded() const;

signals:
    void iconChanged() const;
    void iconSizeChanged(const QSize& size) const;
    void menuWidthChanged(int width) const;
    void animatedChanged(bool animated) const;
    void expandedChanged(bool expanded) const;
    void triggered(QAction* action) const;

public slots:
    QAction* addMenuAction(QAction* action);
    QAction* addMenuAction(const QString& label);
    QAction* addMenuAction(const QIcon& icon, const QString& label);
    void     removeMenuAction(QAction* action);

    void setMenuIcon(const QIcon& icon);
    void setIconSize(const QSize& size);
    void setMenuWidth(int width);
    void setAnimated(bool animated);
    void setExpanded(bool expanded);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void registerAction(QAction* action);
    void unRegisterAction(QAction* action);
    void setExpansionState(bool expanded);

    QActionGroup* mActions;
    QPushButton*  mMenuButton;
    int           mMenuWidth;
    bool          mAnimated;
};

#endif // SIDEBAR_H
