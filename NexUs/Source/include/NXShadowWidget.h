#ifndef NX_SHADOWWIDGET_H_
#define NX_SHADOWWIDGET_H_
#include <QWidget>
#include "NXDef.h"

class NXShadowGraphicsEffect;

class NX_EXPORT NXShadowWidget : public QWidget
{
  Q_OBJECT
  Q_PRIVATE_CREATE_Q_EX_H(const QColor&, QColor, LightColor)
  Q_PRIVATE_CREATE_Q_EX_H(const QColor&, QColor, DarkColor)
  Q_PRIVATE_CREATE_Q_H(NXShadowGraphicsEffectType::RotateMode, RotateMode)
  Q_PRIVATE_CREATE_Q_H(NXShadowGraphicsEffectType::ProjectionMode, ProjectionMode)
  Q_PRIVATE_CREATE_Q_H(qreal, Blur)
  Q_PRIVATE_CREATE_Q_H(qreal, Spread)
  Q_PRIVATE_CREATE_Q_H(QPointF, LightOffset)
  Q_PRIVATE_CREATE_Q_H(QPointF, DarkOffset)
public:
  explicit NXShadowWidget(QWidget *parent = nullptr);
  ~NXShadowWidget() override = default;

  void setCustomDraw(std::function<void(QPainter *, QWidget *)> customDraw);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  NXShadowGraphicsEffect *_pShadowEffect;
  std::function<void(QPainter *, QWidget *)> _pCustomDraw;
};

#endif // !NX_SHADOWWIDGET_H_
