#ifndef NXLINEEDITSTYLE_H
#define NXLINEEDITSTYLE_H

#include <QProxyStyle>

#include "NXDef.h"

class NXLineEditStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PRIVATE_CREATE(int, LineEditBorderRadius)
  Q_PRIVATE_CREATE(int, LineEditIconMargin)
public:
  explicit NXLineEditStyle(QStyle *style = nullptr);
  ~NXLineEditStyle() override;
  void drawPrimitive(PrimitiveElement element,
                     const QStyleOption *option,
                     QPainter *painter,
                     const QWidget *widget = nullptr) const override;
  int pixelMetric(PixelMetric metric,
                  const QStyleOption *option = nullptr,
                  const QWidget *widget      = nullptr) const override;

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXLINEEDITSTYLE_H
