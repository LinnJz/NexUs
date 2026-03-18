#include <QPainter>
#include "NXShadowGraphicsEffect.h"
#include "NXTheme.h"
#include "private/NXShadowGraphicsEffectPrivate.h"

Q_PROPERTY_CREATE_CPP(NXShadowGraphicsEffect, NXShadowGraphicsEffectType::RotateMode, RotateMode)
Q_PROPERTY_CREATE_CPP(NXShadowGraphicsEffect, NXShadowGraphicsEffectType::ProjectionMode, ProjectionMode)
Q_PROPERTY_CREATE_CPP(NXShadowGraphicsEffect, qreal, Blur)
Q_PROPERTY_CREATE_CPP(NXShadowGraphicsEffect, qreal, Spread)
Q_PROPERTY_CREATE_CPP(NXShadowGraphicsEffect, QPointF, LightOffset)
Q_PROPERTY_CREATE_CPP(NXShadowGraphicsEffect, QPointF, DarkOffset)
Q_PROPERTY_CREATE_2_CPP(NXShadowGraphicsEffect, const QColor&, QColor, LightColor)
Q_PROPERTY_CREATE_2_CPP(NXShadowGraphicsEffect, const QColor&, QColor, DarkColor)

NXShadowGraphicsEffect::NXShadowGraphicsEffect(QObject *parent /*= nullptr*/)
    : QGraphicsEffect { parent }
    , d_ptr(new NXShadowGraphicsEffectPrivate())
{
  Q_D(NXShadowGraphicsEffect);
  d->q_ptr            = this;
  d->_pBlur           = 0.0;
  d->_pSpread         = 0.0;
  d->_pLightColor     = NXThemeColor(d->_themeMode, BasicBaseAlpha);
  d->_pDarkColor      = NXThemeColor(d->_themeMode, BasicBaseAlpha);
  d->_pRotateMode     = NXShadowGraphicsEffectType::RotateMode::Rotate45;
  d->_pProjectionMode = NXShadowGraphicsEffectType::ProjectionMode::Inset;
  d->_pLightOffset    = QPointF { 0.0, 0.0 };
  d->_pDarkOffset     = QPointF { 0.0, 0.0 };
  /*NXShadowGraphicsEffect* shadow = new NXShadowGraphicsEffect(this);
  shadow->setBlur(30.0);
  shadow->setLightColor(QColor(0, 0x1E, 0x9A, 102));
  shadow->setDarkColor(Constant::SWITCH_CIRCLE_DARK_SHADOW_COLOR);
  shadow->setLightOffset({ -5,-5 });
  shadow->setDarkOffset({ 5,5 });
  shadow->setProjectionMode(NXShadowGraphicsEffectType::ProjectionMode::Outset);
  shadow->setRotateMode(NXShadowGraphicsEffectType::RotateMode::Rotate45);
  setGraphicsEffect(shadow);*/
}

NXShadowGraphicsEffect::~NXShadowGraphicsEffect() { }

QRectF NXShadowGraphicsEffect::boundingRectFor(const QRectF& rect) const
{
  Q_D(const NXShadowGraphicsEffect);

  // 对于内阴影，不需要扩展边界
  if (d->_pProjectionMode == NXShadowGraphicsEffectType::ProjectionMode::Inset)
  {
    return rect.united(rect.translated(0, 0));
  }

  // 对于外阴影，需要扩展边界以容纳阴影
  qreal boundary = d->_pBlur + d->_pSpread * M_SQRT1_2;

  // 无论是矩形还是圆形阴影，都需要在所有方向上扩展边界
  return rect.united(rect.translated(0, 0).adjusted(-boundary, -boundary, boundary, boundary));
}

void NXShadowGraphicsEffect::draw(QPainter *painter)
{
  Q_D(NXShadowGraphicsEffect);
  QPoint pos;
  const QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &pos, PadToEffectiveBoundingRect);
  if (pixmap.isNull()) return;
  if (d->_pBlur <= 0.0) return;
  QTransform restoreTransform = painter->worldTransform();
  painter->setWorldTransform(QTransform());
  painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
  painter->setPen(Qt::NoPen);

  if (d->_pProjectionMode == NXShadowGraphicsEffectType::ProjectionMode::Inset)
  {
    d->_drawInsetShadow(painter, pixmap, pos);
  }
  else
  {
    d->_drawOutsetShadow(painter, pixmap, pos);
  }

  painter->setWorldTransform(restoreTransform);
}
