#ifndef NXCHECKBOXSTYLE_H
#define NXCHECKBOXSTYLE_H

#include <QProxyStyle>

#include "NXDef.h"

class NXCheckBoxStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PRIVATE_CREATE(int, CheckIndicatorWidth)
  Q_PRIVATE_CREATE(int, CheckBorderRadius)
public:
  explicit NXCheckBoxStyle(QStyle *style = nullptr);
  ~NXCheckBoxStyle() override;
  void drawControl(ControlElement element,
                   const QStyleOption *option,
                   QPainter *painter,
                   const QWidget *widget = nullptr) const override;
  int pixelMetric(PixelMetric metric,
                  const QStyleOption *option = nullptr,
                  const QWidget *widget      = nullptr) const override;

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXCHECKBOXSTYLE_H
