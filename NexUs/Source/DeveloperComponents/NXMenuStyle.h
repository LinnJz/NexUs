#ifndef NXMENUSTYLE_H
#define NXMENUSTYLE_H

#include <QProxyStyle>

#include "NXDef.h"

class NXMenuStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PROPERTY_CREATE(int, MenuItemHeight);
  Q_PROPERTY_CREATE(int, BorderRadius);

public:
  explicit NXMenuStyle(QStyle *style = nullptr);
  ~NXMenuStyle() override;
  void drawPrimitive(PrimitiveElement element,
                     const QStyleOption *option,
                     QPainter *painter,
                     const QWidget *widget = nullptr) const override;
  void drawControl(ControlElement element,
                   const QStyleOption *option,
                   QPainter *painter,
                   const QWidget *widget = nullptr) const override;
  int pixelMetric(PixelMetric metric,
                  const QStyleOption *option = nullptr,
                  const QWidget *widget      = nullptr) const override;
  QSize sizeFromContents(ContentsType type,
                         const QStyleOption *option,
                         const QSize& size,
                         const QWidget *widget) const override;

private:
  mutable int _iconWidth { 0 };
  int _shadowBorderWidth { 6 };
  NXThemeType::ThemeMode _themeMode;
  mutable bool _isAnyoneItemHasIcon { false };
};

#endif // NXMENUSTYLE_H
