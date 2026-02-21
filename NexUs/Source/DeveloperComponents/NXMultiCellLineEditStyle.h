#ifndef NXMULTICELLLINEEDITSTYLE_H
#define NXMULTICELLLINEEDITSTYLE_H

#include <QProxyStyle>

#include "NXDef.h"

class NXMultiCellLineEditStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PRIVATE_CREATE(int, BorderRadius)
public:
  explicit NXMultiCellLineEditStyle(QStyle *style = nullptr);
  ~NXMultiCellLineEditStyle() override;
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

#endif // NXMULTICELLLINEEDITSTYLE_H
