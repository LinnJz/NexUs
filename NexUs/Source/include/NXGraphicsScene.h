#ifndef NXGRAPHICSSCENE_H
#define NXGRAPHICSSCENE_H

#include <QDataStream>
#include <QEvent>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QObject>
#include <QPainter>

#include "NXDef.h"
#include "NXProperty.h"

class NXGraphicsItem;
class NXGraphicsScenePrivate;

class NX_EXPORT NXGraphicsScene : public QGraphicsScene
{
  Q_OBJECT
  Q_Q_CREATE(NXGraphicsScene)
  Q_PROPERTY_CREATE_Q_H(bool, IsCheckLinkPort)
  Q_PROPERTY_CREATE_Q_H(QString, SerializePath)
public:
  explicit NXGraphicsScene(QObject *parent = nullptr);
  ~NXGraphicsScene();
  void addItem(NXGraphicsItem *item);
  void removeItem(NXGraphicsItem *item);
  void removeSelectedItems();
  void clear();

  QList<NXGraphicsItem *> createAndAddItem(int width, int height, int count = 1);
  QList<NXGraphicsItem *> getSelectedNXItems() const;
  QList<NXGraphicsItem *> getNXItems();
  QList<NXGraphicsItem *> getNXItems(QPoint pos);
  QList<NXGraphicsItem *> getNXItems(QPointF pos);
  QList<NXGraphicsItem *> getNXItems(QRect rect);
  QList<NXGraphicsItem *> getNXItems(QRectF rect);

  void setSceneMode(NXGraphicsSceneType::SceneMode mode);
  NXGraphicsSceneType::SceneMode getSceneMode() const;

  void selectAllItems();

  QList<QVariantMap> getItemLinkList() const;
  bool addItemLink(NXGraphicsItem *item1, NXGraphicsItem *item2, int port1 = 0, int port2 = 0);
  bool removeItemLink(NXGraphicsItem *item1);
  bool removeItemLink(NXGraphicsItem *item1, NXGraphicsItem *item2, int port1 = 0, int port2 = 0);

  QList<QVariantMap> getItemsDataRoute() const;

  // 序列化 反序列化
  void serialize();
  void deserialize();

Q_SIGNALS:
  void showItemLink();
  void mouseLeftClickedItem(NXGraphicsItem *item);
  void mouseRightClickedItem(NXGraphicsItem *item);
  void mouseDoubleClickedItem(NXGraphicsItem *item);

protected:
  virtual void focusOutEvent(QFocusEvent *event) override;
  virtual void keyPressEvent(QKeyEvent *event) override;
  virtual void keyReleaseEvent(QKeyEvent *event) override;
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // NXGRAPHICSSCENE_H
