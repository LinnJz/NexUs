#ifndef NXSCROLLBAR_H
#define NXSCROLLBAR_H

#include <QAbstractScrollArea>
#include <QScrollBar>

#include "NXProperty.h"

class NXScrollBarPrivate;

class NX_EXPORT NXScrollBar : public QScrollBar
{
  Q_OBJECT
  Q_Q_CREATE(NXScrollBar)
  Q_PROPERTY_CREATE_H(bool, IsAnimation)
  Q_PROPERTY_CREATE_H(qreal, SpeedLimit)

public:
  explicit NXScrollBar(QWidget *parent = nullptr);
  explicit NXScrollBar(Qt::Orientation orientation, QWidget *parent = nullptr);
  explicit NXScrollBar(QScrollBar *originScrollBar, QAbstractScrollArea *parent = nullptr);
  ~NXScrollBar() override;

Q_SIGNALS:
  void rangeAnimationFinished();

protected:
  bool event(QEvent *event) override;
  bool eventFilter(QObject *watched, QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // NXSCROLLBAR_H
