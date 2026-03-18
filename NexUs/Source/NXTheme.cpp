#include "NXTheme.h"

#include <QPainter>
#include <QPainterPath>

#include "private/NXThemePrivate.h"

NXTheme::NXTheme(QObject *parent)
    : QObject { parent }
    , d_ptr(new NXThemePrivate())
{
  Q_D(NXTheme);
  d->q_ptr = this;
  d->_initThemeColor();
}

NXTheme::~NXTheme() { }

void NXTheme::setThemeMode(NXThemeType::ThemeMode themeMode) noexcept
{
  Q_D(NXTheme);
  d->_themeMode = themeMode;
  Q_EMIT themeModeChanged(d->_themeMode);
}

NXThemeType::ThemeMode NXTheme::getThemeMode() const noexcept
{
  Q_D(const NXTheme);
  return d->_themeMode;
}

void NXTheme::setThemeColor(NXThemeType::ThemeMode themeMode,
                            NXThemeType::ThemeColor themeColor,
                            const QColor& newColor) noexcept
{
  Q_D(NXTheme);
  if (themeMode == NXThemeType::Light) { d->_lightThemeColorList[themeColor] = newColor; }
  else
  {
    d->_darkThemeColorList[themeColor] = newColor;
  }
}

const QColor& NXTheme::getThemeColor(NXThemeType::ThemeMode themeMode,
                                     NXThemeType::ThemeColor themeColor) const noexcept
{
  Q_D(const NXTheme);
  if (themeMode == NXThemeType::Light) { return d->_lightThemeColorList[themeColor]; }
  else
  {
    return d->_darkThemeColorList[themeColor];
  }
}

void NXTheme::drawEffectShadow(QPainter *painter,
                               QRect widgetRect,
                               int shadowBorderWidth,
                               int borderRadius,
                               int maxAlpha,
                               int extendPixels,
                               const QColor& lightColor,
                               const QColor& darkColor) noexcept
{
  Q_D(NXTheme);
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing);
  painter->setPen(Qt::NoPen);

  QColor color = d->_themeMode == NXThemeType::Light ? lightColor : darkColor;

  QPainterPath path;
  path.setFillRule(Qt::WindingFill);

  for (int i = 0; i < shadowBorderWidth; ++i)
  {
    int expansion          = i + 1;
    qreal currentRadius    = borderRadius + expansion / 2;
    int currentBorderWidth = shadowBorderWidth - i;

    path.addRoundedRect(widgetRect.x() + currentBorderWidth, widgetRect.y() + currentBorderWidth,
                        widgetRect.width() - currentBorderWidth * 2, widgetRect.height() - currentBorderWidth * 2,
                        currentRadius, currentRadius);
    int alpha = maxAlpha * currentBorderWidth / shadowBorderWidth;
    color.setAlpha(alpha);

    painter->setBrush(color);
    painter->drawPath(path);
  }
  painter->restore();
}
