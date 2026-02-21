#ifndef NXPROMOTIONVIEWPRIVATE_H
#define NXPROMOTIONVIEWPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "NXDef.h"
class NXPromotionCard;
class NXPromotionView;

class NXPromotionViewPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXPromotionView)
  Q_PROPERTY_CREATE_D(int, CardExpandWidth)
  Q_PROPERTY_CREATE_D(int, CardCollapseWidth)
  Q_PROPERTY_CREATE_D(int, CurrentIndex)
  Q_PROPERTY_CREATE_D(int, AutoScrollInterval)
  Q_PROPERTY_CREATE_D(bool, IsAutoScroll)
public:
  explicit NXPromotionViewPrivate(QObject *parent = nullptr);
  ~NXPromotionViewPrivate();
  void onPromotionCardClicked(NXPromotionCard *clickedCard);

private:
  bool _isAllowSwitch { true };
  NXThemeType::ThemeMode _themeMode;
  int _leftPadding { 40 };
  int _cardSpacing { 5 };
  int _bottomMargin { 30 };
  int _indicatorSpacing { 12 };

  QTimer *_autoScrollTimer { nullptr };
  QList<NXPromotionCard *> _promotionCardList;

  void _startCardGeometryAnimation(NXPromotionCard *card, QRect start, QRect end);
  void _startHorizontalCardPixmapRatioAnimation(NXPromotionCard *card, qreal start, qreal end);
  void _updatePromotionCardGeometry();
  int _getAdjacentIndex(Qt::LayoutDirection direction, int index);
  int _getRightLimitX();
};

#endif // NXPROMOTIONVIEWPRIVATE_H
