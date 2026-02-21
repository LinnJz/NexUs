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

  void addLeftFixedButton(QWidget *widget);
  void addRightFixedButton(QWidget *widget);
  void removeLeftFixedButton(QWidget *widget);
  void removeRightFixedButton(QWidget *widget);
  void clearLeftFixedButtons();
  void clearRightFixedButtons();

Q_SIGNALS:
  Q_SIGNAL void tabDragCreate(QMimeData *mimeData);
  Q_SIGNAL void tabDragEnter(QMimeData *mimeData);
  Q_SIGNAL void tabDragLeave(QMimeData *mimeData);
  Q_SIGNAL void tabDragDrop(QMimeData *mimeData);

protected:
  QSize sizeHint() const;
  void tabInserted(int index) override;
  void tabRemoved(int index) override;
  void tabLayoutChange() override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dragLeaveEvent(QDragLeaveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
};

#endif // NXTABBAR_H
