#ifndef NXPIVOT_H
#define NXPIVOT_H
#include <QWidget>

#include "NXProperty.h"

class NXPivotPrivate;

class NX_EXPORT NXPivot : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXPivot)
  Q_PROPERTY_CREATE_Q_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_Q_H(int, CurrentIndex)
  Q_PROPERTY_CREATE_Q_H(int, PivotSpacing)
  Q_PROPERTY_CREATE_Q_H(int, MarkWidth)
public:
  explicit NXPivot(QWidget *parent = nullptr);
  ~NXPivot();

  void appendPivot(const QString& pivotTitle);
  void removePivot(const QString& pivotTitle);

Q_SIGNALS:
  Q_SIGNAL void pivotClicked(int index);
  Q_SIGNAL void pivotDoubleClicked(int index);
};

#endif // NXPIVOT_H
