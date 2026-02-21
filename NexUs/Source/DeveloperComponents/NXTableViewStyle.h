#ifndef NXTABLEVIEWSTYLE_H
#define NXTABLEVIEWSTYLE_H
#include <QModelIndex>
#include <QProxyStyle>

#include "NXDef.h"
struct NXAdjustParam;
class QStyleOptionViewItem;

class NXTableViewStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PROPERTY_CREATE(QModelIndex, CurrentHoverIndex)
  Q_PROPERTY_CREATE(int, HeaderMargin)
  Q_PROPERTY_CREATE(int, BorderRadius)
  Q_PROPERTY_CREATE(bool, IsDrawAlternateRowsEnabled)
  Q_PROPERTY_CREATE(bool, IsHoverEffectsEnabled)
  Q_PROPERTY_CREATE(bool, IsSelectionEffectsEnabled)
public:
  using coords = std::tuple<int, int, int, int>;
  explicit NXTableViewStyle(QStyle *style = nullptr);
  ~NXTableViewStyle() override;
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

  void adjustHeaderColumnIconRect(const QHash<int, coords>& adjusts);
  void adjustColummTextRect(const QHash<int, coords>& adjusts);

private:
  int _horizontalPadding { 11 };
  NXThemeType::ThemeMode _themeMode;
  int _borderRadius { 3 };
  QHash<int, coords> _headerAdjusts;
  QHash<int, coords> _adjusts;
};

#endif // NXTABLEVIEWSTYLE_H
