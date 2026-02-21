#ifndef NXSCROLLPAGEAREA_H
#define NXSCROLLPAGEAREA_H

#include <QWidget>

#include "NXProperty.h"
class NXScrollPageAreaPrivate;

class NX_EXPORT NXScrollPageArea : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXScrollPageArea)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
  explicit NXScrollPageArea(QWidget *parent = nullptr);
  ~NXScrollPageArea() override;

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXSCROLLPAGEAREA_H
