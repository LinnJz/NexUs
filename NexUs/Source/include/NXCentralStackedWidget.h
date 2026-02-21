#ifndef NXCENTRALSTACKEDWIDGET_H
#define NXCENTRALSTACKEDWIDGET_H

#include <QStackedWidget>

#include <QVBoxLayout>
#include "NXDef.h"
class QGraphicsBlurEffect;

class NXCentralStackedWidget : public QWidget
{
  Q_OBJECT
  Q_PROPERTY_CREATE(int, BlurAnimationRadius)
  Q_PROPERTY_CREATE(int, PopupAnimationYOffset)
  Q_PROPERTY_CREATE(qreal, ScaleAnimationRatio)
  Q_PROPERTY_CREATE(qreal, ScaleAnimationPixOpacity)
  Q_PROPERTY_CREATE(qreal, FlipAnimationRatio)
public:
  explicit NXCentralStackedWidget(QWidget *parent = nullptr);
  ~NXCentralStackedWidget() override;

  QStackedWidget *getContainerStackedWidget() const;

  void setCustomWidget(QWidget *widget);
  QWidget *getCustomWidget() const;

  Q_SLOT void onThemeModeChanged(NXThemeType::ThemeMode themeMode);

  void setIsTransparent(bool isTransparent);
  bool getIsTransparent() const;

  void setIsHasRadius(bool isHasRadius);

  void doWindowStackSwitch(NXWindowType::StackSwitchMode stackSwitchMode, int nodeIndex, bool isRouteBack);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  NXWindowType::StackSwitchMode _stackSwitchMode { NXWindowType::StackSwitchMode::Popup };
  NXThemeType::ThemeMode _themeMode;
  QPixmap _targetStackPix;
  QPixmap _currentStackPix;
  QGraphicsBlurEffect *_blurEffect { nullptr };
  bool _isTransparent { false };
  QVBoxLayout *_mainLayout { nullptr };
  QWidget *_customWidget { nullptr };
  QStackedWidget *_containerStackedWidget { nullptr };
  bool _isHasRadius { true };
  bool _isDrawNewPix { false };
  void _getTargetStackPix();
  void _getCurrentStackPix();
};

#endif // NXCENTRALSTACKEDWIDGET_H
