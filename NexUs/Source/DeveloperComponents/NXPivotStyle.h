#ifndef NXPIVOTSTYLE_H
#define NXPIVOTSTYLE_H

#include <QModelIndex>
#include <QPixmap>
#include <QProxyStyle>
#include <QStyleOption>

#include "NXDef.h"
#include "NXPivotModel.h"

class NXPivotView;

class NXPivotStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PRIVATE_CREATE_2(const QColor &, QColor, TextNormalColor)
  Q_PRIVATE_CREATE_2(const QColor &, QColor, TextFocusColor)
  Q_PRIVATE_CREATE_2(const QColor &, QColor, TextPressedColor)
  Q_PRIVATE_CREATE_2(const QColor &, QColor, MarkColor)
  Q_PRIVATE_CREATE_2(const QColor &, QColor, MarkBackgroundColor)
  Q_PRIVATE_CREATE(bool, IsAutoAdaptivePivotHeight)
  Q_PRIVATE_CREATE(bool, IsPivotFixedSize)
  Q_PRIVATE_CREATE(bool, IsHoverBackgroundEnabled)
  Q_PRIVATE_CREATE(NXPivotType::DisplayMode, DisplayMode)
  Q_PRIVATE_CREATE(NXPivotType::MarkFlags, MarkFlags)
  Q_PRIVATE_CREATE(int, PivotHorizontalSpacing)
  Q_PRIVATE_CREATE(int, PivotVerticalSpacing)
  Q_PRIVATE_CREATE(int, TextIconSpacing)
  Q_PRIVATE_CREATE(QPoint, OverlayTextOffsetFromCenterAnchor)
  Q_PRIVATE_CREATE(QSize, PivotFixedSize)

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
  int
  pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const override;

  QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;

  QSize sizeFromContents(ContentsType type,
                         const QStyleOption *option,
                         const QSize &size,
                         const QWidget *widget) const override;

private:
  void _drawCenteredPixmap(QPainter *painter, const QRect &containerRect, const QPixmap &pixmap) const noexcept;
  void _drawTextOnly(QPainter *painter, const QRect &contentRect, const QString &text) const noexcept;
  void _drawIconOnly(QPainter *painter, const QRect &contentRect, const QPixmap &icon) const noexcept;
  void _drawTextBesideIcon(QPainter *painter,
                           const QRect &contentRect,
                           const QPixmap &icon,
                           const QString &text) const noexcept;
  void _drawTextUnderIcon(QPainter *painter,
                          const QRect &contentRect,
                          const QPixmap &icon,
                          const QString &text) const noexcept;
  void _drawIconWithOverlayText(QPainter *painter,
                                const QRect &contentRect,
                                const QPixmap &icon,
                                const QString &text) const noexcept;
  void _drawDisplayContent(QPainter *painter,
                           const QRect &contentRect,
                           const NXPivotModel::PivotItem &pivotItem) const noexcept;
  void _drawFocusRectBackground(QPainter *painter, const QRect &rect, const QColor &color) const noexcept;
  void _drawPivotMark(QPainter *painter, const QRect &rect, const QColor &color) const noexcept;

  void _parentAutoAdaptivePivotHeight(const NXPivotView *pivotView, int actualHeight) const noexcept;
  std::pair<QSize, int> _getAdaptiveSizeByPivotItem(const NXPivotModel::PivotItem &pivotItem,
                                                    const QFontMetrics &metrics,
                                                    bool hasIcon,
                                                    bool hasText) const noexcept;
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXPIVOTSTYLE_H
