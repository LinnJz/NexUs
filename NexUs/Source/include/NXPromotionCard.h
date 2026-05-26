#ifndef NXPROMOTIONCARD_H
#define NXPROMOTIONCARD_H
#include <QWidget>

#include "NXProperty.h"
class NXPromotionCardPrivate;

class NX_EXPORT NXPromotionCard : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXPromotionCard)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QPixmap), CardPixmap)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), CardTitle)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), PromotionTitle)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Title)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), SubTitle)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), CardTitleColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), PromotionTitleColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), PromotionTitleBaseColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), TitleColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), SubTitleColor)
  Q_PROPERTY_CREATE_H(qreal, HorizontalCardPixmapRatio)
  Q_PROPERTY_CREATE_H(qreal, VerticalCardPixmapRatio)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, CardTitlePixelSize)
  Q_PROPERTY_CREATE_H(int, PromotionTitlePixelSize)
  Q_PROPERTY_CREATE_H(int, TitlePixelSize)
  Q_PROPERTY_CREATE_H(int, SubTitlePixelSize)

public:
  explicit NXPromotionCard(QWidget *parent = nullptr);
  ~NXPromotionCard();
  Q_SIGNAL void promotionCardClicked();

protected:
  bool event(QEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXPROMOTIONCARD_H
