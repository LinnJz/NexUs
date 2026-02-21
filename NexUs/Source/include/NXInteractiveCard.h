#ifndef NXINTERACTIVECARD_H
#define NXINTERACTIVECARD_H

#include <QPixmap>
#include <QPushButton>

#include "NXDef.h"

class NXInteractiveCardPrivate;

class NX_EXPORT NXInteractiveCard : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXInteractiveCard)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize);
  Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize);
  Q_PROPERTY_CREATE_Q_H(int, TitleSpacing);
  Q_PROPERTY_CREATE_Q_H(int, CardPixmapBorderRadius)
  Q_PROPERTY_CREATE_Q_H(NXCardPixType::PixMode, CardPixMode);
  Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap);
  Q_PROPERTY_CREATE_Q_H(QSize, CardPixmapSize);
  Q_PROPERTY_CREATE_Q_H(QString, Title);
  Q_PROPERTY_CREATE_Q_H(QString, SubTitle);

public:
  explicit NXInteractiveCard(QWidget *parent = nullptr);
  ~NXInteractiveCard();
  void setCardPixmapSize(int width, int height);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXINTERACTIVECARD_H
