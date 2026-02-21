#ifndef NXFLOWLAYOUTPRIVATE_H
#define NXFLOWLAYOUTPRIVATE_H

#include <QLayout>
#include <QMap>
#include <QObject>
#include <QStyle>

#include "NXProperty.h"
class NXFlowLayout;

class NXFlowLayoutPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXFlowLayout)
public:
  explicit NXFlowLayoutPrivate(QObject *parent = nullptr);
  ~NXFlowLayoutPrivate() override;

private:
  int _doLayout(const QRect& rect, bool testOnly) const;
  int _smartSpacing(QStyle::PixelMetric pm) const;
  mutable QMap<QLayoutItem *, QPoint> _lastGeometryMap;
  QList<QLayoutItem *> _itemList;
  bool _isAnimation { false };
  int _hSpacing;
  int _vSpacing;
};

#endif // NXFLOWLAYOUTPRIVATE_H
