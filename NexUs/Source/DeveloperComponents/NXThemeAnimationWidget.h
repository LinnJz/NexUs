#ifndef NXTHEMEANIMATIONWIDGET_H
#define NXTHEMEANIMATIONWIDGET_H

#include <QWidget>

#include "NXProperty.h"

class NXThemeAnimationWidget : public QWidget
{
  Q_OBJECT
  Q_PROPERTY_CREATE(QS_SET_CREF(QImage), OldWindowBackground)
  Q_PROPERTY_CREATE(qreal, Radius)
  Q_PROPERTY_CREATE(qreal, EndRadius)
  Q_PROPERTY_CREATE(QPoint, Center)

public:
  explicit NXThemeAnimationWidget(QWidget *parent = nullptr);
  ~NXThemeAnimationWidget();
  void startAnimation(int msec);
  Q_SIGNAL void animationFinished();

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXTHEMEANIMATIONWIDGET_H
