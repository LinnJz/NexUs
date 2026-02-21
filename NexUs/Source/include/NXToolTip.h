#ifndef NXTOOLTIP_H
#define NXTOOLTIP_H

#include <QWidget>

#include "NXProperty.h"
class NXToolTipPrivate;

class NX_EXPORT NXToolTip : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXToolTip)
  Q_PROPERTY_CREATE_Q_H(int, OffSetX)
  Q_PROPERTY_CREATE_Q_H(int, OffSetY)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, DisplayMsec)
  Q_PROPERTY_CREATE_Q_H(int, ShowDelayMsec)
  Q_PROPERTY_CREATE_Q_H(int, HideDelayMsec)
  Q_PROPERTY_CREATE_Q_H(QString, ToolTip)
  Q_PROPERTY_CREATE_Q_H(QWidget *, CustomWidget)

public:
  explicit NXToolTip(QWidget *parent = nullptr);
  ~NXToolTip() override;

  void updatePos(const QPoint& pos);

protected:
  virtual void paintEvent(QPaintEvent *event);
};

#endif // NXTOOLTIP_H
