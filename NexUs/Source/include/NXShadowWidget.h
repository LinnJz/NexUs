#ifndef NX_SHADOWWIDGET_H_
#define NX_SHADOWWIDGET_H_
#include <QWidget>
#include "NXDef.h"

class NXShadowWidgetPrivate;

class NX_EXPORT NXShadowWidget : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXShadowWidget)
  Q_PROPERTY_CREATE_H(NXShadowGraphicsEffectType::RotateMode, RotateMode)
  Q_PROPERTY_CREATE_H(NXShadowGraphicsEffectType::ProjectionMode, ProjectionMode)
  Q_PROPERTY_CREATE_H(qreal, Blur)
  Q_PROPERTY_CREATE_H(qreal, Spread)
  Q_PROPERTY_CREATE_H(QPointF, LightOffset)
  Q_PROPERTY_CREATE_H(QPointF, DarkOffset)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, LightColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, DarkColor)

public:
  explicit NXShadowWidget(QWidget *parent = nullptr);
  ~NXShadowWidget() override = default;

  void setCustomDraw(std::function<void(QPainter *, QWidget *)> customDraw) noexcept;

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // !NX_SHADOWWIDGET_H_
