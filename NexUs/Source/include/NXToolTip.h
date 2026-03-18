#ifndef NXTOOLTIP_H
#define NXTOOLTIP_H

#include <QWidget>

#include "NXProperty.h"
class NXToolTipPrivate;

class NX_EXPORT NXToolTip : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXToolTip)
  Q_PROPERTY_CREATE_H(bool, IsMoveEnabled)
  Q_PROPERTY_CREATE_H(int, OffSetX)
  Q_PROPERTY_CREATE_H(int, OffSetY)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, DisplayMsec)
  Q_PROPERTY_CREATE_H(int, ShowDelayMsec)
  Q_PROPERTY_CREATE_H(int, HideDelayMsec)
  Q_PROPERTY_CREATE_H(QWidget *, CustomWidget)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, ToolTip)

public:
  explicit NXToolTip(QWidget *parent = nullptr);
  ~NXToolTip() override;
  void updatePos() noexcept;

protected:
  void paintEvent(QPaintEvent *event);
};

#endif // NXTOOLTIP_H
