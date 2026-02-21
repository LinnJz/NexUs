#ifndef NXPIVOTSTYLE_H
#define NXPIVOTSTYLE_H

#include <QModelIndex>
#include <QProxyStyle>

#include "NXDef.h"

class NXPivotStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PRIVATE_CREATE(QModelIndex, PressIndex)
  Q_PRIVATE_CREATE(int, CurrentIndex)
  Q_PRIVATE_CREATE(int, PivotSpacing)
public:
  explicit NXPivotStyle(QStyle *style = nullptr);
  ~NXPivotStyle() override;
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

  const QColor& getMarkColor();

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXPIVOTSTYLE_H
