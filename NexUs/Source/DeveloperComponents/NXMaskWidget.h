#ifndef NXMASKWIDGET_H
#define NXMASKWIDGET_H

#include <QWidget>

#include "NXProperty.h"

class NXMaskWidget : public QWidget
{
  Q_OBJECT
  Q_PROPERTY_CREATE(int, MaskAlpha)
public:
  explicit NXMaskWidget(QWidget *parent = nullptr);
  ~NXMaskWidget();
  void doMaskAnimation(int endValue);

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXMASKWIDGET_H
