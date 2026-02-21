#ifndef NXGRAPHICSVIEW_H
#define NXGRAPHICSVIEW_H

#include <QGraphicsView>

#include "NXProperty.h"
class NXGraphicsViewPrivate;

class NX_EXPORT NXGraphicsView : public QGraphicsView
{
  Q_OBJECT
  Q_Q_CREATE(NXGraphicsView)
  Q_PROPERTY_CREATE_Q_H(qreal, MaxTransform);
  Q_PROPERTY_CREATE_Q_H(qreal, MinTransform);

public:
  explicit NXGraphicsView(QWidget *parent = nullptr);
  explicit NXGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
  ~NXGraphicsView();

protected:
  void wheelEvent(QWheelEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // NXGRAPHICSVIEW_H
