#ifndef NXIMAGECARD_H
#define NXIMAGECARD_H

#include <QWidget>

#include "NXProperty.h"

class NXImageCardPrivate;

class NX_EXPORT NXImageCard : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXImageCard)
  Q_PROPERTY_CREATE_Q_H(bool, IsPreserveAspectCrop)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(QImage, CardImage);

public:
  explicit NXImageCard(QWidget *parent = nullptr);
  ~NXImageCard() override;

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXIMAGECARD_H
