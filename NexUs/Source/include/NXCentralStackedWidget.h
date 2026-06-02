#ifndef NXCENTRALSTACKEDWIDGET_H
#define NXCENTRALSTACKEDWIDGET_H

#include "NXDef.h"
#include "NXLazyStackedWidget.h"

class NXCentralStackedWidgetPrivate;

class NX_EXPORT NXCentralStackedWidget : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXCentralStackedWidget)
  Q_PROPERTY_CREATE_H(int, LastTargetIndex)
  Q_PROPERTY_CREATE_H(int, BlurAnimationRadius)
  Q_PROPERTY_CREATE_H(int, PopupAnimationYOffset)
  Q_PROPERTY_CREATE_H(qreal, ScaleAnimationRatio)
  Q_PROPERTY_CREATE_H(qreal, ScaleAnimationPixOpacity)
  Q_PROPERTY_CREATE_H(qreal, FlipAnimationRatio)

public:
  explicit NXCentralStackedWidget(QWidget *parent = nullptr);
  ~NXCentralStackedWidget() override;

  NXLazyStackedWidget *getContainerStackedWidget() const;

  void setCustomWidget(QWidget *widget);
  QWidget *getCustomWidget() const;

  void setIsTransparent(bool isTransparent);
  bool getIsTransparent() const;

  void setIsHasRadius(bool isHasRadius);

  void doWindowStackSwitch(NXWindowType::StackSwitchMode stackSwitchMode, int nodeIndex, bool isRouteBack);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXCENTRALSTACKEDWIDGET_H
