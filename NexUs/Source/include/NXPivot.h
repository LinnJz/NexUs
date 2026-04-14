#ifndef NXPIVOT_H
#define NXPIVOT_H

#include <QPixmap>
#include <QPoint>
#include <QWidget>

#include "NXDef.h"

class NXPivotPrivate;

class NX_EXPORT NXPivot : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXPivot)
  Q_PROPERTY_CREATE_2_H(const QColor &, QColor, TextNormalColor)
  Q_PROPERTY_CREATE_2_H(const QColor &, QColor, TextFocusColor)
  Q_PROPERTY_CREATE_2_H(const QColor &, QColor, TextPressedColor)
  Q_PROPERTY_CREATE_2_H(const QColor &, QColor, MarkColor)
  Q_PROPERTY_CREATE_2_H(const QColor &, QColor, MarkBackgroundColor)
  Q_PROPERTY_CREATE_H(QPoint, OverlayTextOffsetFromCenterAnchor)
  Q_PROPERTY_CREATE_H(QSize, PivotFixedSize)
  Q_PROPERTY_CREATE_H(NXPivotType::DisplayMode, DisplayMode)
  Q_PROPERTY_CREATE_H(NXPivotType::MarkFlags, MarkFlags)
  Q_PROPERTY_CREATE_H(int, TextIconSpacing)
  Q_PROPERTY_CREATE_H(int, PivotHorizontalSpacing)
  Q_PROPERTY_CREATE_H(int, PivotVerticalSpacing)
  Q_PROPERTY_CREATE_H(bool, IsAutoAdaptivePivotHeight)
  Q_PROPERTY_CREATE_H(bool, IsPivotFixedSize)
  Q_PROPERTY_CREATE_H(bool, IsHoverBackgroundEnabled)

  Q_PROPERTY_CREATE_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_H(int, CurrentIndex)
  Q_PROPERTY_CREATE_H(int, MarkWidth)
  Q_PROPERTY_CREATE_H(Qt::CursorShape, HoverValidIndexCursor)

public:
  explicit NXPivot(QWidget *parent = nullptr);
  ~NXPivot();

  void appendPivot(const QPixmap &pixmap) noexcept;
  void appendPivot(const QString &pivotTitle, const QPixmap &pixmap = {}) noexcept;

  void insertPivot(int index, const QPixmap &pixmap) noexcept;
  void insertPivot(int index, const QString &pivotTitle, const QPixmap &pixmap = {}) noexcept;

  void removePivot(int index) noexcept;

  void setPivot(int index, const QPixmap &pixmap) noexcept;
  void setPivot(int index, const QString &pivotTitle, const QPixmap &pixmap = {}) noexcept;

Q_SIGNALS:
  void pivotChanged(int index);
  void pivotInserted(int index);
  void pivotRemoved(int index);

  void pivotClicked(int index);
  void pivotDoubleClicked(int index);
};

#endif // NXPIVOT_H
