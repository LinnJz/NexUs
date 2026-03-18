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
  Q_PROPERTY_CREATE_H(bool, IsCheckLinkPort)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, SerializePath)

public:
  explicit NXGraphicsScene(QObject *parent = nullptr);
  ~NXGraphicsScene();
  void addItem(NXGraphicsItem *item) noexcept;
  void removeItem(NXGraphicsItem *item) noexcept;
  void removeSelectedItems() noexcept;
  void clear() noexcept;

  QList<NXGraphicsItem *> createAndAddItem(int width, int height, int count = 1) noexcept;
  QList<NXGraphicsItem *> getSelectedNXItems() const noexcept;
  QList<NXGraphicsItem *> getNXItems() noexcept;
  QList<NXGraphicsItem *> getNXItems(QPoint pos) noexcept;
  QList<NXGraphicsItem *> getNXItems(QPointF pos) noexcept;
  QList<NXGraphicsItem *> getNXItems(QRect rect) noexcept;
  QList<NXGraphicsItem *> getNXItems(QRectF rect) noexcept;

  void setSceneMode(NXGraphicsSceneType::SceneMode mode) noexcept;
  NXGraphicsSceneType::SceneMode getSceneMode() const noexcept;

  void selectAllItems() noexcept;

  QList<QVariantMap> getItemLinkList() const noexcept;
  bool addItemLink(NXGraphicsItem *item1, NXGraphicsItem *item2, int port1 = 0, int port2 = 0) noexcept;
  bool removeItemLink(NXGraphicsItem *item1) noexcept;
  bool removeItemLink(NXGraphicsItem *item1, NXGraphicsItem *item2, int port1 = 0, int port2 = 0) noexcept;

  QList<QVariantMap> getItemsDataRoute() const noexcept;

  // 序列化 反序列化
  void serialize() noexcept;
  void deserialize() noexcept;

Q_SIGNALS:
  void showItemLink();
  void mouseLeftClickedItem(NXGraphicsItem *item);
  void mouseRightClickedItem(NXGraphicsItem *item);
  void mouseDoubleClickedItem(NXGraphicsItem *item);

protected:
  void focusOutEvent(QFocusEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // NXGRAPHICSSCENE_H
