#ifndef NXGRAPHICSITEM_H
#define NXGRAPHICSITEM_H

#include <QGraphicsObject>
#include <QJsonObject>
#include <QObject>
#include <QPainter>

#include "NXProperty.h"
class NXGraphicsScene;
class NXGraphicsItemPrivate;

class NX_EXPORT NXGraphicsItem : public QGraphicsObject
{
  Q_OBJECT
  Q_Q_CREATE(NXGraphicsItem)
  Q_PROPERTY_CREATE_H(int, Width)
  Q_PROPERTY_CREATE_H(int, Height)
  Q_PROPERTY_CREATE_H(int, MaxLinkPortCount)
  Q_PROPERTY_CREATE_2_H(const QImage&, QImage, ItemImage)
  Q_PROPERTY_CREATE_2_H(const QImage&, QImage, ItemSelectedImage)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, ItemName)
  Q_PROPERTY_CREATE_2_H(const QVariantMap&, QVariantMap, DataRoutes)

public:
  explicit NXGraphicsItem(QGraphicsItem *parent = nullptr);
  explicit NXGraphicsItem(int width, int height, QGraphicsItem *parent = nullptr);
  ~NXGraphicsItem();

  QString getItemUID() const noexcept;

  void setLinkPortState(bool isFullLink) noexcept;
  void setLinkPortState(bool isLink, int portIndex) noexcept;

  bool getLinkPortState(int portIndex) const noexcept;
  QList<bool> getLinkPortState() const noexcept;

  int getUsedLinkPortCount() const noexcept;
  QList<int> getUsedLinkPort() const noexcept;
  int getUnusedLinkPortCount() const noexcept;
  QList<int> getUnusedLinkPort() const noexcept;

protected:
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
  friend QDataStream& operator<< (QDataStream& stream, const NXGraphicsItem *item);
  friend QDataStream& operator>> (QDataStream& stream, NXGraphicsItem *item);
};

#endif // NXGRAPHICSITEM_H
