#ifndef NXCENTRALSTACKEDWIDGET_H
#define NXCENTRALSTACKEDWIDGET_H

#include <QStackedWidget>

#include "NXDef.h"

class NXCentralStackedWidgetPrivate;

class NXCentralStackedWidget : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXCentralStackedWidget)
  Q_PROPERTY_CREATE_H(int, BlurAnimationRadius)
  Q_PROPERTY_CREATE_H(int, PopupAnimationYOffset)
  Q_PROPERTY_CREATE_H(qreal, ScaleAnimationRatio)
  Q_PROPERTY_CREATE_H(qreal, ScaleAnimationPixOpacity)
  Q_PROPERTY_CREATE_H(qreal, FlipAnimationRatio)

public:
  explicit NXCentralStackedWidget(QWidget *parent = nullptr);
  ~NXCentralStackedWidget() override;

  QStackedWidget *getContainerStackedWidget() const noexcept;

  void setCustomWidget(QWidget *widget) noexcept;
  QWidget *getCustomWidget() const noexcept;

  void setIsTransparent(bool isTransparent) noexcept;
  bool getIsTransparent() const noexcept;

  void setIsHasRadius(bool isHasRadius) noexcept;

  void doWindowStackSwitch(NXWindowType::StackSwitchMode stackSwitchMode, int nodeIndex, bool isRouteBack) noexcept;

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXCENTRALSTACKEDWIDGET_H
