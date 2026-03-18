#include "NXShadowWidget.h"

#include <QPainter>
#include <QPainterPath>
#include "NXShadowGraphicsEffect.h"
#include "private/NXShadowWidgetPrivate.h"

NXShadowWidget::NXShadowWidget(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , d_ptr(new NXShadowWidgetPrivate())
{
  Q_D(NXShadowWidget);
  d->q_ptr = this;

  setAttribute(Qt::WA_TranslucentBackground);
  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
  setStyleSheet(QStringLiteral("background-color: transparent;"));
  setFixedSize(200, 200);
  d->_shadowEffect = new NXShadowGraphicsEffect(this);
  setGraphicsEffect(d->_shadowEffect);
}

void NXShadowWidget::setCustomDraw(std::function<void(QPainter *, QWidget *)> customDraw) noexcept
{
  d_ptr->_customDraw = customDraw;
}

void NXShadowWidget::setDarkOffset(QPointF size) noexcept
{
  d_ptr->_shadowEffect->setDarkOffset(size);
  update();
}

QPointF NXShadowWidget::getDarkOffset() const noexcept { return d_ptr->_shadowEffect->getDarkOffset(); }

void NXShadowWidget::setLightOffset(QPointF size) noexcept
{
  d_ptr->_shadowEffect->setLightOffset(size);
  update();
}

QPointF NXShadowWidget::getLightOffset() const noexcept { return d_ptr->_shadowEffect->getLightOffset(); }

void NXShadowWidget::setRotateMode(NXShadowGraphicsEffectType::RotateMode mode) noexcept
{
  d_ptr->_shadowEffect->setRotateMode(mode);
  update();
}

NXShadowGraphicsEffectType::RotateMode NXShadowWidget::getRotateMode() const noexcept
{
  return d_ptr->_shadowEffect->getRotateMode();
}

void NXShadowWidget::setProjectionMode(NXShadowGraphicsEffectType::ProjectionMode mode) noexcept
{
  d_ptr->_shadowEffect->setProjectionMode(mode);
  update();
}

NXShadowGraphicsEffectType::ProjectionMode NXShadowWidget::getProjectionMode() const noexcept
{
  return d_ptr->_shadowEffect->getProjectionMode();
}

void NXShadowWidget::setBlur(qreal blur) noexcept
{
  d_ptr->_shadowEffect->setBlur(blur);
  update();
}

qreal NXShadowWidget::getBlur() const noexcept { return d_ptr->_shadowEffect->getBlur(); }

void NXShadowWidget::setSpread(qreal spread) noexcept
{
  d_ptr->_shadowEffect->setSpread(spread);
  update();
}

qreal NXShadowWidget::getSpread() const noexcept { return d_ptr->_shadowEffect->getSpread(); }

void NXShadowWidget::setLightColor(const QColor& color) noexcept
{
  d_ptr->_shadowEffect->setLightColor(color);
  update();
}

QColor NXShadowWidget::getLightColor() const noexcept { return d_ptr->_shadowEffect->getLightColor(); }

void NXShadowWidget::setDarkColor(const QColor& color) noexcept
{
  d_ptr->_shadowEffect->setDarkColor(color);
  update();
}

QColor NXShadowWidget::getDarkColor() const noexcept { return d_ptr->_shadowEffect->getDarkColor(); }

void NXShadowWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  if (d_ptr->_shadowEffect->getProjectionMode() == NXShadowGraphicsEffectType::ProjectionMode::Outset)
  {
    if (d_ptr->_customDraw) { d_ptr->_customDraw(&painter, this->parentWidget()); }
    else
    {
      painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
      painter.setPen(Qt::NoPen);
      painter.setBrush(QColor { 235, 239, 243, 255 });
      painter.drawRect(rect());
    }
  }
  QWidget::paintEvent(event);
}

