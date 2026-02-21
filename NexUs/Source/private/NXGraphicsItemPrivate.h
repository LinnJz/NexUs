#ifndef NXGRAPHICSITEMPRIVATE_H
#define NXGRAPHICSITEMPRIVATE_H

#include <QList>
#include <QObject>

#include "NXGraphicsItem.h"

class NXGraphicsItemPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXGraphicsItem)
  Q_PROPERTY_CREATE_D(int, Width)
  Q_PROPERTY_CREATE_D(int, Height)
  Q_PROPERTY_CREATE_D(int, MaxLinkPortCount)
  Q_PROPERTY_CREATE_D(QImage, ItemImage)
  Q_PROPERTY_CREATE_D(QImage, ItemSelectedImage)
  Q_PROPERTY_CREATE_D(QString, ItemName)
  Q_PROPERTY_CREATE_D(QVariantMap, DataRoutes)
public:
  explicit NXGraphicsItemPrivate(QObject *parent = nullptr);
  ~NXGraphicsItemPrivate();

  friend QDataStream& operator<<(QDataStream& stream, const NXGraphicsItemPrivate *data);
  friend QDataStream& operator>>(QDataStream& stream, NXGraphicsItemPrivate *data);

private:
  QString _itemUID;
  QList<bool> _currentLinkPortState;
};

#endif // NXGRAPHICSITEMPRIVATE_H
