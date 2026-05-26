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
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QImage), ItemImage)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QImage), ItemSelectedImage)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), ItemName)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QVariantMap), DataRoutes)
  Q_PROPERTY_CREATE_H(int, Width)
  Q_PROPERTY_CREATE_H(int, Height)
  Q_PROPERTY_CREATE_H(int, MaxLinkPortCount)

public:
  explicit NXGraphicsItem(QGraphicsItem *parent = nullptr);
  explicit NXGraphicsItem(int width, int height, QGraphicsItem *parent = nullptr);
  ~NXGraphicsItem();

  QString getItemUID() const;

  void setLinkPortState(bool isFullLink);
  void setLinkPortState(bool isLink, int portIndex);

  bool getLinkPortState(int portIndex) const;
  QVector<bool> getLinkPortState() const;

  int getUsedLinkPortCount() const;
  QVector<int> getUsedLinkPort() const;
  int getUnusedLinkPortCount() const;
  QVector<int> getUnusedLinkPort() const;

protected:
  QRectF boundingRect() const override;
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
  friend QDataStream &operator<< (QDataStream &stream, const NXGraphicsItem *item);
  friend QDataStream &operator>> (QDataStream &stream, NXGraphicsItem *item);
};

#endif // NXGRAPHICSITEM_H
