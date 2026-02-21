#ifndef NXGRAPHICSVIEWPRIVATE_H
#define NXGRAPHICSVIEWPRIVATE_H

#include <QObject>

#include "NXProperty.h"
class NXGraphicsView;

class NXGraphicsViewPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXGraphicsView)
  Q_PROPERTY_CREATE_D(qreal, MaxTransform);
  Q_PROPERTY_CREATE_D(qreal, MinTransform);

public:
  explicit NXGraphicsViewPrivate(QObject *parent = nullptr);
  ~NXGraphicsViewPrivate();

private:
  void _initStyle();
};

#endif // NXGRAPHICSVIEWPRIVATE_H
