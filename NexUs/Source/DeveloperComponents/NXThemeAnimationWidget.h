#ifndef NXTHEMEANIMATIONWIDGET_H
#define NXTHEMEANIMATIONWIDGET_H

#include <QWidget>

#include "NXProperty.h"

class NXThemeAnimationWidget : public QWidget
{
  Q_OBJECT
  Q_PROPERTY_CREATE(qreal, Radius)
  Q_PROPERTY_CREATE(qreal, EndRadius)
  Q_PROPERTY_CREATE(QPoint, Center)
  Q_PROPERTY_CREATE(QImage, OldWindowBackground)
public:
  explicit NXThemeAnimationWidget(QWidget *parent = nullptr);
  ~NXThemeAnimationWidget() override;
  void startAnimation(int msec);
Q_SIGNALS:
  Q_SIGNAL void animationFinished();

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXTHEMEANIMATIONWIDGET_H
