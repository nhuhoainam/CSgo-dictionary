#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QAction>
#include <QWidget>

class Sidebar : public QWidget {
  Q_OBJECT
public:
  explicit Sidebar(QWidget *parent = nullptr);
  void addAction(QAction *action);
  QAction *addAction(const QString &text, const QIcon &icon = QIcon());
  void setSidebarIcon(QIcon &icon);
  QSize minimumSizeHint() const;

signals:
  void actionSelected(int i);

public slots:
  void setSelected(int i, bool selected);

protected:
  void paintEvent(QPaintEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void leaveEvent(QEvent *event);

  QAction *actionAt(const QPoint &at);
  int indexAt(const QPoint &at);

private:
  QList<QAction *> mActions;

  QAction *mCheckedAction;
  QAction *mOverAction;
  int mSidebarWidth;
  QSize mIconSize;
  QIcon *mSidebarIcon;
};

#endif // SIDEBAR_H
