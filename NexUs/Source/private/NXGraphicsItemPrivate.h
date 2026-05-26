#ifndef NXGRAPHICSITEMPRIVATE_H
#define NXGRAPHICSITEMPRIVATE_H

#include <QObject>
#include <QVector>

#include "NXGraphicsItem.h"

class NXGraphicsItemPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXGraphicsItem)
  Q_PROPERTY_CREATE_D(QImage, ItemImage)
  Q_PROPERTY_CREATE_D(QImage, ItemSelectedImage)
  Q_PROPERTY_CREATE_D(QString, ItemName)
  Q_PROPERTY_CREATE_D(QVariantMap, DataRoutes)
  Q_PROPERTY_CREATE_D(int, Width)
  Q_PROPERTY_CREATE_D(int, Height)
  Q_PROPERTY_CREATE_D(int, MaxLinkPortCount)

public:
  friend QDataStream &operator<< (QDataStream &stream, const NXGraphicsItemPrivate *data);
  friend QDataStream &operator>> (QDataStream &stream, NXGraphicsItemPrivate *data);
  explicit NXGraphicsItemPrivate(QObject *parent = nullptr);
  ~NXGraphicsItemPrivate();


private:
  QString _itemUID;
  QVector<bool> _currentLinkPortState;
};

#endif // NXGRAPHICSITEMPRIVATE_H
