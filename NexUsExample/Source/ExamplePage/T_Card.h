#ifndef T_CARD_H
#define T_CARD_H

#include "T_BasePage.h"

class NXLCDNumber;
class NXPromotionCard;
class NXPromotionView;

class T_Card : public T_BasePage
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit T_Card(QWidget *parent = nullptr);
  ~T_Card();

private:
  NXLCDNumber *_lcdNumber { nullptr };
  NXPromotionCard *_promotionCard { nullptr };
  NXPromotionView *_promotionView { nullptr };
};

#endif // T_CARD_H
