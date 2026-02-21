#ifndef NXSCROLLBARSTYLE_H
#define NXSCROLLBARSTYLE_H
#include <QProxyStyle>

#include "NXDef.h"
class NXScrollBar;

class NXScrollBarStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PRIVATE_CREATE(bool, IsExpand)
  Q_PROPERTY_CREATE(qreal, Opacity)
  Q_PROPERTY_CREATE(qreal, SliderExtent)
  Q_PRIVATE_CREATE(NXScrollBar *, ScrollBar)
public:
  explicit NXScrollBarStyle(QStyle *style = nullptr);
  ~NXScrollBarStyle() override;
  void drawComplexControl(ComplexControl control,
                          const QStyleOptionComplex *option,
                          QPainter *painter,
                          const QWidget *widget = nullptr) const override;
  int pixelMetric(PixelMetric metric,
                  const QStyleOption *option = nullptr,
                  const QWidget *widget      = nullptr) const override;
  int styleHint(StyleHint hint,
                const QStyleOption *option   = nullptr,
                const QWidget *widget        = nullptr,
                QStyleHintReturn *returnData = nullptr) const override;
  void startExpandAnimation(bool isExpand);

private:
  NXThemeType::ThemeMode _themeMode;
  int _scrollBarExtent { 10 };
  qreal _sliderMargin { 2.5 };
};

#endif // NXSCROLLBARSTYLE_H
