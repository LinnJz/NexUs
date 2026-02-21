#ifndef NXPROMOTIONCARDPRIVATE_H
#define NXPROMOTIONCARDPRIVATE_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

#include "NXProperty.h"
class QRadialGradient;
class NXPromotionCard;

class NXPromotionCardPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXPromotionCard)
  Q_PROPERTY_CREATE_D(QPixmap, CardPixmap)
  Q_PROPERTY_CREATE_D(QString, CardTitle)
  Q_PROPERTY_CREATE_D(QString, PromotionTitle)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, SubTitle)
  Q_PROPERTY_CREATE_D(QColor, CardTitleColor)
  Q_PROPERTY_CREATE_D(QColor, PromotionTitleColor)
  Q_PROPERTY_CREATE_D(QColor, PromotionTitleBaseColor)
  Q_PROPERTY_CREATE_D(QColor, TitleColor)
  Q_PROPERTY_CREATE_D(QColor, SubTitleColor)
  Q_PROPERTY_CREATE_D(qreal, HorizontalCardPixmapRatio)
  Q_PROPERTY_CREATE_D(qreal, VerticalCardPixmapRatio)
  Q_PROPERTY_CREATE(qreal, PressRadius)
  Q_PROPERTY_CREATE(qreal, HoverOpacity)
  Q_PROPERTY_CREATE(qreal, PressOpacity)
  Q_PROPERTY_CREATE_D(int, CardTitlePixelSize)
  Q_PROPERTY_CREATE_D(int, PromotionTitlePixelSize)
  Q_PROPERTY_CREATE_D(int, TitlePixelSize)
  Q_PROPERTY_CREATE_D(int, SubTitlePixelSize)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
  explicit NXPromotionCardPrivate(QObject *parent = nullptr);
  ~NXPromotionCardPrivate();

private:
  int _shadowBorderWidth { 6 };
  bool _isPressAnimationFinished { true };
  QRadialGradient *_hoverGradient { nullptr };
  QRadialGradient *_pressGradient { nullptr };

  qreal _getLongestDistance(QPoint point);
  qreal _distance(QPoint point1, QPoint point2);
  void _startHoverOpacityAnimation(bool isVisible);
};

#endif // NXPROMOTIONCARDPRIVATE_H
