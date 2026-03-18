#ifndef NXPIVOT_H
#define NXPIVOT_H
#include <QWidget>

#include "NXProperty.h"

class NXPivotPrivate;

class NX_EXPORT NXPivot : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXPivot)
  Q_PROPERTY_CREATE_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_H(int, CurrentIndex)
  Q_PROPERTY_CREATE_H(int, PivotSpacing)
  Q_PROPERTY_CREATE_H(int, MarkWidth)

public:
  explicit NXPivot(QWidget *parent = nullptr);
  ~NXPivot();

  void appendPivot(const QString& pivotTitle) noexcept;
  void removePivot(const QString& pivotTitle) noexcept;

Q_SIGNALS:
  void pivotClicked(int index);
  void pivotDoubleClicked(int index);
};

#endif // NXPIVOT_H
