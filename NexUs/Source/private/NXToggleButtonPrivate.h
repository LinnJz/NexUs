#ifndef NXTOGGLEBUTTONPRIVATE_H
#define NXTOGGLEBUTTONPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXToggleButton;

class NXToggleButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXToggleButton)
  Q_PROPERTY_CREATE_D(QString, Text)
  Q_PROPERTY_CREATE_D(NXIconType::IconName, NXIcon, NXIconType::None)
  Q_PROPERTY_CREATE_D(int, BorderRadius, 3)
  Q_PROPERTY_CREATE(int, ToggleAlpha, QS_DEFAULT_VALUE(0))

public:
  explicit NXToggleButtonPrivate(QObject *parent = nullptr);
  ~NXToggleButtonPrivate();

private:
  bool _isAlphaAnimationFinished { true };
  bool _isToggled { false };
  bool _isPressed { false };
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXTOGGLEBUTTONPRIVATE_H
