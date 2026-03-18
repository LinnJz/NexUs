#ifndef NXTOGGLESWITCHPRIVATE_H
#define NXTOGGLESWITCHPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXProperty.h"
class NXToggleSwitch;

class NXToggleSwitchPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXToggleSwitch);

public:
  explicit NXToggleSwitchPrivate(QObject *parent = nullptr);
  ~NXToggleSwitchPrivate();

private:
  bool _isToggled { false };
  bool _isLeftButtonPress { false };
  bool _isMousePressMove { false };
  int _margin { 1 };
  int _lastMouseX { 0 };
  NXThemeType::ThemeMode _themeMode;
  qreal _circleCenterX { 0 };
  qreal _circleRadius { 0 };

  void _startPosAnimation(qreal startX, qreal endX, bool isToggle) noexcept;
  void _startRadiusAnimation(qreal startRadius, qreal endRadius) noexcept;
  void _adjustCircleCenterX() noexcept;
};

#endif // NXTOGGLESWITCHPRIVATE_H
