#ifndef NXTABLEVIEWSTYLE_H
#define NXTABLEVIEWSTYLE_H
#include <QModelIndex>
#include <QProxyStyle>

#include "NXDef.h"
class QStyleOptionViewItem;

class NXTableViewStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PROPERTY_CREATE_EX(const QModelIndex&, QModelIndex, CurrentHoverIndex)
  Q_PROPERTY_CREATE(int, HeaderMargin)
  Q_PROPERTY_CREATE(int, BorderRadius)
  Q_PROPERTY_CREATE(int, CheckIndicatorWidth)
  Q_PROPERTY_CREATE(bool, IsHoverEffectsEnabled)
  Q_PROPERTY_CREATE(bool, IsSelectionEffectsEnabled)

public:
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

  void setHorizontalPadding(int column, int padding);
  int getHorizontalPadding(int column) const;
  void syncHorizontalPaddings(int columnCount);

private:
  NXThemeType::ThemeMode _themeMode;
  QList<int> _horizontalPaddings;

  int _horizontalPaddingForColumn(int column) const;
  void _drawCheckIndicator(QPainter *painter, const QRect& rect, Qt::CheckState state) const;
};

#endif // NXTABLEVIEWSTYLE_H
