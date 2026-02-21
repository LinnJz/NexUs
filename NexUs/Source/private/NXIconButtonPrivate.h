#ifndef NXICONBUTTONPRIVATE_H
#define NXICONBUTTONPRIVATE_H

#include <QColor>
#include <QObject>
#include <QPixmap>

#include "NXDef.h"
class NXIconButton;

class NXIconButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXIconButton)
  Q_PROPERTY_CREATE_D(qreal, Opacity);
  Q_PROPERTY_CREATE_D(QColor, LightHoverColor);
  Q_PROPERTY_CREATE_D(QColor, DarkHoverColor);
  Q_PROPERTY_CREATE_D(QColor, LightIconColor);
  Q_PROPERTY_CREATE_D(QColor, DarkIconColor);
  Q_PROPERTY_CREATE_D(QColor, LightHoverIconColor);
  Q_PROPERTY_CREATE_D(QColor, DarkHoverIconColor);
  bool _isAlphaAnimationFinished { true };
  Q_PROPERTY_CREATE_D(bool, IsSelected);
  Q_PROPERTY_CREATE_D(NXIconType::IconName, Awesome)
  Q_PROPERTY_CREATE(int, HoverAlpha)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
  explicit NXIconButtonPrivate(QObject *parent = nullptr);
  ~NXIconButtonPrivate();

private:
  NXThemeType::ThemeMode _themeMode;
  QPixmap _iconPix;
};

#endif // NXICONBUTTONPRIVATE_H
