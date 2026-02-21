#ifndef NXPROMOTIONCARD_H
#define NXPROMOTIONCARD_H
#include <QWidget>

#include "NXProperty.h"
class NXPromotionCardPrivate;

class NX_EXPORT NXPromotionCard : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXPromotionCard)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, CardTitlePixelSize)
  Q_PROPERTY_CREATE_Q_H(int, PromotionTitlePixelSize)
  Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize)
  Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize)
  Q_PROPERTY_CREATE_Q_H(qreal, HorizontalCardPixmapRatio)
  Q_PROPERTY_CREATE_Q_H(qreal, VerticalCardPixmapRatio)
  Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap)
  Q_PROPERTY_CREATE_Q_H(QString, CardTitle)
  Q_PROPERTY_CREATE_Q_H(QString, PromotionTitle)
  Q_PROPERTY_CREATE_Q_H(QString, Title)
  Q_PROPERTY_CREATE_Q_H(QString, SubTitle)
  Q_PROPERTY_CREATE_Q_H(QColor, CardTitleColor)
  Q_PROPERTY_CREATE_Q_H(QColor, PromotionTitleColor)
  Q_PROPERTY_CREATE_Q_H(QColor, PromotionTitleBaseColor)
  Q_PROPERTY_CREATE_Q_H(QColor, TitleColor)
  Q_PROPERTY_CREATE_Q_H(QColor, SubTitleColor)
public:
  explicit NXPromotionCard(QWidget *parent = nullptr);
  ~NXPromotionCard();
Q_SIGNALS:
  Q_SIGNAL void promotionCardClicked();

protected:
  virtual bool event(QEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXPROMOTIONCARD_H
