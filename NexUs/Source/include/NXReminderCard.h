#ifndef NXREMINDERCARD_H
#define NXREMINDERCARD_H

#include <QPushButton>

#include "NXDef.h"
class NXReminderCardPrivate;

class NX_EXPORT NXReminderCard : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXReminderCard)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize);
  Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize);
  Q_PROPERTY_CREATE_Q_H(int, TitleSpacing);
  Q_PROPERTY_CREATE_Q_H(int, CardPixmapBorderRadius)
  Q_PROPERTY_CREATE_Q_H(NXCardPixType::PixMode, CardPixMode);
  Q_PROPERTY_CREATE_Q_H(QString, Title);
  Q_PROPERTY_CREATE_Q_H(QString, SubTitle);
  Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap);
  Q_PROPERTY_CREATE_Q_H(QSize, CardPixmapSize);

public:
  explicit NXReminderCard(QWidget *parent = nullptr);
  ~NXReminderCard();
  void setCardPixmapSize(int width, int height);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXREMINDERCARD_H
