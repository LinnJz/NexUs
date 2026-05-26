#ifndef NXPUSHBUTTONPRIVATE_H
#define NXPUSHBUTTONPRIVATE_H

#include <QColor>
#include <QObject>

#include "NXDef.h"
class NXPushButton;

class NXPushButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXPushButton)
  Q_PROPERTY_CREATE_D(QColor, LightDefaultColor)
  Q_PROPERTY_CREATE_D(QColor, DarkDefaultColor)
  Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
  Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
  Q_PROPERTY_CREATE_D(QColor, LightPressColor)
  Q_PROPERTY_CREATE_D(QColor, DarkPressColor)
  Q_PROPERTY_CREATE_D(QColor, LightTextColor)
  Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(bool, IsHoverEnable)

public:
  explicit NXPushButtonPrivate(QObject *parent = nullptr);
  ~NXPushButtonPrivate();

private:
  bool _isPressed { false };
  bool _hasIcon { false };
  int _shadowBorderWidth { 3 };
  int _iconSize { 18 };
  NXThemeType::ThemeMode _themeMode;
  NXIconType::IconName _icon { NXIconType::None };
  NXTextType::TextStyle _textStyle { NXTextType::NoStyle };
};

#endif // NXPUSHBUTTONPRIVATE_H
