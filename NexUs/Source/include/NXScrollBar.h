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
  Q_PROPERTY_CREATE_Q_H(bool, IsAnimation)
  Q_PROPERTY_CREATE_Q_H(qreal, SpeedLimit)
public:
  explicit NXScrollBar(QWidget *parent = nullptr);
  explicit NXScrollBar(Qt::Orientation orientation, QWidget *parent = nullptr);
  explicit NXScrollBar(QScrollBar *originScrollBar, QAbstractScrollArea *parent = nullptr);
  ~NXScrollBar() override;

Q_SIGNALS:
  Q_SIGNAL void rangeAnimationFinished();

protected:
  virtual bool event(QEvent *event) override;
  virtual bool eventFilter(QObject *watched, QEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void wheelEvent(QWheelEvent *event) override;
  virtual void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // NXSCROLLBAR_H
