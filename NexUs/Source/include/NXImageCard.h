#ifndef NXIMAGECARD_H
#define NXIMAGECARD_H

#include <QWidget>

#include "NXProperty.h"

class NXImageCardPrivate;

class NX_EXPORT NXImageCard : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXImageCard)
  Q_PROPERTY_CREATE_H(bool, IsPreserveAspectCrop)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_2_H(const QImage&, QImage, CardImage)

public:
  explicit NXImageCard(QWidget *parent = nullptr);
  ~NXImageCard() override;

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXIMAGECARD_H
