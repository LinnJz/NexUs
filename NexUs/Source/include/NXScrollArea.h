#ifndef NXSCROLLAREA_H
#define NXSCROLLAREA_H

#include <QScrollArea>

#include "NXProperty.h"

class NXScrollAreaPrivate;

class NX_EXPORT NXScrollArea : public QScrollArea
{
  Q_OBJECT
  Q_Q_CREATE(NXScrollArea)
public:
  explicit NXScrollArea(QWidget *parent = nullptr);
  ~NXScrollArea();

  void setIsGrabGesture(bool isEnable, qreal mousePressEventDelay = 0.5);

  void setIsOverShoot(Qt::Orientation orientation, bool isEnable);
  bool getIsOverShoot(Qt::Orientation orientation) const;

  void setIsAnimation(Qt::Orientation orientation, bool isAnimation);
  bool getIsAnimation(Qt::Orientation orientation) const;
};

#endif // NXSCROLLAREA_H
