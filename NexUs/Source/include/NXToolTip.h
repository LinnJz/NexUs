#ifndef NXTOOLTIP_H
#define NXTOOLTIP_H

#include <QWidget>

#include "NXProperty.h"
class NXToolTipPrivate;

class NX_EXPORT NXToolTip : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXToolTip)
  Q_PROPERTY_CREATE_Q_H(bool, IsMoveEnabled)
  Q_PROPERTY_CREATE_Q_H(int, OffSetX)
  Q_PROPERTY_CREATE_Q_H(int, OffSetY)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, DisplayMsec)
  Q_PROPERTY_CREATE_Q_H(int, ShowDelayMsec)
  Q_PROPERTY_CREATE_Q_H(int, HideDelayMsec)
  Q_PROPERTY_CREATE_Q_H(QWidget *, CustomWidget)
  Q_PROPERTY_CREATE_Q_EX_H(const QString&, QString, ToolTip)
public:
  explicit NXToolTip(QWidget *parent = nullptr);
  ~NXToolTip() override;
  void updatePos();

protected:
  virtual void paintEvent(QPaintEvent *event);
};

#endif // NXTOOLTIP_H
