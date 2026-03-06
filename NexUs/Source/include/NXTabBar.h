#ifndef NXTABBAR_H
#define NXTABBAR_H

#include <QDrag>
#include <QTabBar>

#include "NXDef.h"
class NXTabBarPrivate;

class NX_EXPORT NXTabBar : public QTabBar
{
  Q_OBJECT
  Q_Q_CREATE(NXTabBar)
  Q_PROPERTY_CREATE_Q_H(bool, IsSelectedIndicatorVisible)
  Q_PROPERTY_CREATE_Q_H(int, TabCornerRadius)
  Q_PROPERTY_CREATE_Q_H(NXTabBarType::TabBarStyle, TabBarStyle)
  Q_PROPERTY_CREATE_Q_H(QSize, TabSize)

public:
  explicit NXTabBar(QWidget *parent = nullptr);
  ~NXTabBar() override;
Q_SIGNALS:
  Q_SIGNAL void tabDragCreate(QMimeData *mimeData);
  Q_SIGNAL void tabDragEnter(QMimeData *mimeData);
  Q_SIGNAL void tabDragLeave(QMimeData *mimeData);
  Q_SIGNAL void tabDragDrop(QMimeData *mimeData);

protected:
  QSize sizeHint() const;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void dragEnterEvent(QDragEnterEvent *event) override;
  virtual void dragMoveEvent(QDragMoveEvent *event) override;
  virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
  virtual void dropEvent(QDropEvent *event) override;
  virtual void wheelEvent(QWheelEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXTABBAR_H
