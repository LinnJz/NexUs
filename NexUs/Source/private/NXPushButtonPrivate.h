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

public:
  explicit NXPushButtonPrivate(QObject *parent = nullptr);
  ~NXPushButtonPrivate();

private:
  int _iconSize { 18 };
  int _shadowBorderWidth { 3 };
  NXIconType::IconName _icon { NXIconType::None };
  NXTextType::TextStyle _textStyle { NXTextType::NoStyle };
  NXThemeType::ThemeMode _themeMode;
  bool _hasIcon { false };
  bool _isPressed { false };
};

#endif // NXPUSHBUTTONPRIVATE_H
