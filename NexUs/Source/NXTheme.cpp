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

void NXTheme::setThemeMode(NXThemeType::ThemeMode themeMode)
{
  Q_D(NXTheme);
  d->_themeMode = themeMode;
  Q_EMIT themeModeChanged(d->_themeMode);
}

NXThemeType::ThemeMode NXTheme::getThemeMode() const
{
  Q_D(const NXTheme);
  return d->_themeMode;
}

void NXTheme::drawEffectShadow(QPainter *painter, QRect widgetRect, int shadowBorderWidth, int borderRadius)
{
  Q_D(NXTheme);
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing);
  QPainterPath path;
  path.setFillRule(Qt::WindingFill);
  QColor color = d->_themeMode == NXThemeType::Light ? QColor(0x70, 0x70, 0x70) : QColor(0x9C, 0x9B, 0x9E);
  for (int i = 0; i < shadowBorderWidth; i++)
  {
    path.addRoundedRect(widgetRect.x() + shadowBorderWidth - i,
                        widgetRect.y() + shadowBorderWidth - i,
                        widgetRect.width() - (shadowBorderWidth - i) * 2,
                        widgetRect.height() - (shadowBorderWidth - i) * 2,
                        borderRadius + i,
                        borderRadius + i);
    int alpha = 1 * (shadowBorderWidth - i + 1);
    color.setAlpha(alpha > 255 ? 255 : alpha);
    painter->setPen(color);
    painter->drawPath(path);
  }
  painter->restore();
}

void NXTheme::drawEffectShadow(QPainter *painter,
                               QRect widgetRect,
                               int shadowBorderWidth,
                               int borderRadius,
                               int maxAlpha,
                               int extendPixels,
                               const QColor& lightColor,
                               const QColor& darkColor)
{
  Q_D(NXTheme);
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing);

  QColor color = d->_themeMode == NXThemeType::Light ? lightColor : darkColor;

  for (int i = 0; i < shadowBorderWidth; ++i)
  {
    int expansion = i + extendPixels;

    QRect rect = widgetRect.adjusted(-expansion, -expansion, expansion, expansion);

    int currentRadius = borderRadius + expansion / 2;

    QPainterPath path;
    path.addRoundedRect(rect, currentRadius, currentRadius);

    int alpha = maxAlpha * (shadowBorderWidth - i) / shadowBorderWidth;
    alpha     = qBound(0, alpha, 255);
    color.setAlpha(alpha);

    painter->setBrush(color);
    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
  }
  painter->restore();
}

void NXTheme::setThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor, QColor newColor)
{
  Q_D(NXTheme);
  if (themeMode == NXThemeType::Light) { d->_lightThemeColorList[themeColor] = newColor; }
  else
  {
    d->_darkThemeColorList[themeColor] = newColor;
  }
}

const QColor& NXTheme::getThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor)
{
  Q_D(NXTheme);
  if (themeMode == NXThemeType::Light) { return d->_lightThemeColorList[themeColor]; }
  else
  {
    return d->_darkThemeColorList[themeColor];
  }
}
