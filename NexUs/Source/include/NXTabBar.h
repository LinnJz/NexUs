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
  Q_PROPERTY_CREATE_H(bool, IsSelectedIndicatorVisible)
  Q_PROPERTY_CREATE_H(int, TabCornerRadius)
  Q_PROPERTY_CREATE_H(NXTabBarType::TabBarStyle, TabBarStyle)
  Q_PROPERTY_CREATE_H(QSize, TabSize)

public:
  explicit NXTabBar(QWidget *parent = nullptr);
  ~NXTabBar() override;
Q_SIGNALS:
  void tabDragCreate(QMimeData *mimeData);
  void tabDragEnter(QMimeData *mimeData);
  void tabDragLeave(QMimeData *mimeData);
  void tabDragDrop(QMimeData *mimeData);

protected:
  QSize sizeHint() const;
  void mouseMoveEvent(QMouseEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dragLeaveEvent(QDragLeaveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXTABBAR_H
