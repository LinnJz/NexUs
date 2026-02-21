#ifndef NXRADIOBUTTONSTYLE_H
#define NXRADIOBUTTONSTYLE_H

#include <QProxyStyle>

#include "NXDef.h"

class NXRadioButtonStyle : public QProxyStyle
{
  Q_OBJECT
public:
  explicit NXRadioButtonStyle(QStyle *style = nullptr);
  ~NXRadioButtonStyle() override;
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

#endif // NXRADIOBUTTONSTYLE_H
