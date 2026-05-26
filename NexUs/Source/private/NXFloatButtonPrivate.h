#ifndef NXFLOATBUTTONPRIVATE_H
#define NXFLOATBUTTONPRIVATE_H

#include <QObject>

#include "NXDef.h"

class NXFloatButton;
class NXMenu;

class NXFloatButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXFloatButton)
  Q_PROPERTY_CREATE_D(int, ButtonSize)
  Q_PROPERTY_CREATE_D(int, Margin)
  Q_PROPERTY_CREATE(int, HoverAlpha)

public:
  explicit NXFloatButtonPrivate(QObject *parent = nullptr);
  ~NXFloatButtonPrivate();

  void updatePosition();

private:
  bool _isPressed { false };
  bool _isAlphaAnimationFinished { true };
  NXIconType::IconName _icon { NXIconType::Plus };
  int _position { 0 };
  NXThemeType::ThemeMode _themeMode;
  NXMenu *_menu { nullptr };
};

#endif // NXFLOATBUTTONPRIVATE_H
