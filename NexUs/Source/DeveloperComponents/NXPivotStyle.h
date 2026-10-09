#ifndef NXPIVOTSTYLE_H
#define NXPIVOTSTYLE_H

#include <QPixmap>
#include <QProxyStyle>

#include "NXPivotModel.h"

class NXPivotView;

class NXPivotStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PRIVATE_CREATE(QS_SET_CREF(QColor), TextNormalColor)
  Q_PRIVATE_CREATE(QS_SET_CREF(QColor), TextFocusColor)
  Q_PRIVATE_CREATE(QS_SET_CREF(QColor), TextPressedColor)
  Q_PRIVATE_CREATE(QS_SET_CREF(QColor), IndicatorColor)
  Q_PRIVATE_CREATE(QS_SET_CREF(QColor), ItemBackgroundColor)
  Q_PRIVATE_CREATE(QPoint, OverlayTextOffsetFromCenterAnchor)
  Q_PRIVATE_CREATE(QSize, ItemFixedSize)
  Q_PRIVATE_CREATE(NXPivotType::DisplayMode, DisplayMode, QS_DEFAULT_VALUE(NXPivotType::TextOnly))
  Q_PRIVATE_CREATE(NXPivotType::IndicatorTypeFlags, IndicatorTypeFlags, QS_DEFAULT_VALUE(NXPivotType::IndicatorBottom))
  Q_PRIVATE_CREATE(int, ItemHorizontalSpacing, QS_DEFAULT_VALUE(48))
  Q_PRIVATE_CREATE(int, ItemVerticalSpacing, QS_DEFAULT_VALUE(16))
  Q_PRIVATE_CREATE(int, TextIconSpacing, QS_DEFAULT_VALUE(6))
  Q_PRIVATE_CREATE(bool, IsAutoAdaptiveItemHeight, QS_DEFAULT_VALUE(true))
  Q_PRIVATE_CREATE(bool, IsItemFixedSize, QS_DEFAULT_VALUE(false))
  Q_PRIVATE_CREATE(bool, IsHoverItemBackgroundEnable, QS_DEFAULT_VALUE(false))

public:
  explicit NXPivotStyle(QStyle *style = nullptr);
  ~NXPivotStyle() override;
  void drawPrimitive(PrimitiveElement element,
                     QStyleOption const *option,
                     QPainter *painter,
                     QWidget const *widget = nullptr) const override;
  void drawControl(ControlElement element,
                   QStyleOption const *option,
                   QPainter *painter,
                   QWidget const *widget = nullptr) const override;
  int
  pixelMetric(PixelMetric metric, QStyleOption const *option = nullptr, QWidget const *widget = nullptr) const override;

  QRect subElementRect(SubElement element, QStyleOption const *option, QWidget const *widget) const override;

  QSize sizeFromContents(ContentsType type,
                         QStyleOption const *option,
                         QSize const &size,
                         QWidget const *widget) const override;

private:
  NXThemeType::ThemeMode _themeMode;

  void _drawCenteredPixmap(QPainter *painter, QRect const &containerRect, QPixmap const &pixmap) const;
  void _drawTextOnly(QPainter *painter, QRect const &contentRect, QString const &text) const;
  void _drawIconOnly(QPainter *painter, QRect const &contentRect, QPixmap const &icon) const;
  void _drawTextBesideIcon(QPainter *painter, QRect const &contentRect, QPixmap const &icon, QString const &text) const;
  void _drawTextUnderIcon(QPainter *painter, QRect const &contentRect, QPixmap const &icon, QString const &text) const;
  void
  _drawIconWithOverlayText(QPainter *painter, QRect const &contentRect, QPixmap const &icon, QString const &text) const;
  void _drawDisplayContent(QPainter *painter, QRect const &contentRect, NXPivotModel::PivotItem const &pivotItem) const;
  void _drawFocusRectBackground(QPainter *painter, QRect const &rect, QColor const &color) const;
  void _drawIndicator(QPainter *painter, QRect const &rect, QColor const &color) const;

  void _alignHeight(NXPivotView const *pivotView, int actualHeight) const;
  QSize _calculateItemSize(NXPivotModel::PivotItem const &pivotItem,
                           QFontMetrics const &metrics,
                           bool hasIcon,
                           bool hasText) const;
};

#endif // NXPIVOTSTYLE_H
