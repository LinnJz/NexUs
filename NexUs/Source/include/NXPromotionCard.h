#ifndef NXPROMOTIONCARD_H
#define NXPROMOTIONCARD_H
#include <QWidget>

#include "NXProperty.h"
class NXPromotionCardPrivate;

class NX_EXPORT NXPromotionCard : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXPromotionCard)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, CardTitlePixelSize)
  Q_PROPERTY_CREATE_H(int, PromotionTitlePixelSize)
  Q_PROPERTY_CREATE_H(int, TitlePixelSize)
  Q_PROPERTY_CREATE_H(int, SubTitlePixelSize)
  Q_PROPERTY_CREATE_H(qreal, HorizontalCardPixmapRatio)
  Q_PROPERTY_CREATE_H(qreal, VerticalCardPixmapRatio)
  Q_PROPERTY_CREATE_2_H(const QPixmap&, QPixmap, CardPixmap)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, CardTitle)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, PromotionTitle)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, Title)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, SubTitle)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, CardTitleColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, PromotionTitleColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, PromotionTitleBaseColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, TitleColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, SubTitleColor)

public:
  explicit NXPromotionCard(QWidget *parent = nullptr);
  ~NXPromotionCard();

Q_SIGNALS:
  void promotionCardClicked();

protected:
  bool event(QEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXPROMOTIONCARD_H
