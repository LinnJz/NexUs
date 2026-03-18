#ifndef NXCENTRALSTACKEDWIDGET_PAIVATE_H
#define NXCENTRALSTACKEDWIDGET_PAIVATE_H

#include <QObject>
#include <QPixmap>
#include "NXDef.h"

class QWidget;
class QStackedWidget;
class QVBoxLayout;
class QGraphicsBlurEffect;
class NXCentralStackedWidget;

class NXCentralStackedWidgetPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXCentralStackedWidget)
  Q_PROPERTY_CREATE(int, BlurAnimationRadius)
  Q_PROPERTY_CREATE(int, PopupAnimationYOffset)
  Q_PROPERTY_CREATE(qreal, ScaleAnimationRatio)
  Q_PROPERTY_CREATE(qreal, ScaleAnimationPixOpacity)
  Q_PROPERTY_CREATE(qreal, FlipAnimationRatio)

public:
  explicit NXCentralStackedWidgetPrivate(QObject *parent = nullptr);
  ~NXCentralStackedWidgetPrivate();

  Q_SLOT void onThemeModeChanged(NXThemeType::ThemeMode themeMode) noexcept;

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
  void _getTargetStackPix() noexcept;
  void _getCurrentStackPix() noexcept;
};

#endif // NXCENTRALSTACKEDWIDGET_PAIVATE_H
