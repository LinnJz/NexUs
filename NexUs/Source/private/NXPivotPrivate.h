#ifndef NXPIVOTPRIVATE_H
#define NXPIVOTPRIVATE_H

#include <QObject>

#include "NXProperty.h"

class NXPivot;
class NXPivotModel;
class NXPivotStyle;
class NXPivotView;

class NXPivotPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXPivot)
  Q_PROPERTY_CREATE_D(int, TextPixelSize)
public:
  explicit NXPivotPrivate(QObject *parent = nullptr);
  ~NXPivotPrivate();

private:
  NXPivotModel *_listModel { nullptr };
  NXPivotStyle *_listStyle { nullptr };
  NXPivotView *_listView { nullptr };
  void _checkCurrentIndex();
};

#endif // NXPIVOTPRIVATE_H
