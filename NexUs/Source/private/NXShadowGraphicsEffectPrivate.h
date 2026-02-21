#ifndef NX_SHADOW_GRAPHICSEFFECT_PRIVATE_H_
#define NX_SHADOW_GRAPHICSEFFECT_PRIVATE_H_
#include <QColor>
#include <QObject>
#include <QPointF>

#include "NXDef.h"
class QPainter;
class NXShadowGraphicsEffect;

class NXShadowGraphicsEffectPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXShadowGraphicsEffect)
  Q_PROPERTY_CREATE_D(qreal, Blur)
  Q_PROPERTY_CREATE_D(qreal, Spread)
  Q_PROPERTY_CREATE_D(QColor, LightColor)
  Q_PROPERTY_CREATE_D(QColor, DarkColor)
  Q_PROPERTY_CREATE_D(QPointF, LightOffset)
  Q_PROPERTY_CREATE_D(QPointF, DarkOffset)
  Q_PROPERTY_CREATE_D(NXShadowGraphicsEffectType::RotateMode, RotateMode)
  Q_PROPERTY_CREATE_D(NXShadowGraphicsEffectType::ProjectionMode, ProjectionMode)
public:
  explicit NXShadowGraphicsEffectPrivate(QObject *parent = nullptr);
  ~NXShadowGraphicsEffectPrivate();

private:
  void _drawInsetShadow(QPainter *painter, const QPixmap& pixmap, const QPoint& pos);
  void _drawOutsetShadow(QPainter *painter, const QPixmap& pixmap, const QPoint& pos);

  NXThemeType::ThemeMode _themeMode { NXThemeType::ThemeMode::Light };
};
#endif // !NX_SHADOW_GRAPHICSEFFECT_PRIVATE_H_
