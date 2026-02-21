#ifndef NXGRAPHICSSCENEPRIVATE_H
#define NXGRAPHICSSCENEPRIVATE_H

#include <QMap>
#include <QObject>
#include <QPointF>

#include "NXGraphicsScene.h"
#include "NXProperty.h"
class NXGraphicsItem;
class NXGraphicsLineItem;

class NXGraphicsScenePrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXGraphicsScene)

  Q_PROPERTY_CREATE_D(QString, SerializePath)
  Q_PROPERTY_CREATE_D(bool, IsCheckLinkPort)
public:
  explicit NXGraphicsScenePrivate(QObject *parent = nullptr);
  ~NXGraphicsScenePrivate();

  friend QDataStream& operator<<(QDataStream& stream, const NXGraphicsScenePrivate *data);
  friend QDataStream& operator>>(QDataStream& stream, NXGraphicsScenePrivate *data);

private:
  bool _isLeftButtonPress { false };
  NXGraphicsSceneType::SceneMode _sceneMode;

  qreal _currentZ { 1 };
  QPointF _lastPos;
  QPointF _lastLeftPressPos;
  QList<QVariantMap> _itemsLink; // item连接状态
  QList<NXGraphicsLineItem *> _lineItemsList;
  QMap<QString, NXGraphicsItem *> _items; // 存储所有item
  NXGraphicsLineItem *_linkLineItem { nullptr };


  QList<NXGraphicsItem *> _serializeItem(int count);

  void _removeLinkLineItem();
  void _deserializeLink();
};

#endif // NXGRAPHICSSCENEPRIVATE_H
