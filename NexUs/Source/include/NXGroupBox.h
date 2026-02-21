#ifndef NXGROUPBOX_H
#define NXGROUPBOX_H

#include <QGroupBox>

#include "NXProperty.h"
class NXGroupBoxPrivate;

class NX_EXPORT NXGroupBox : public QGroupBox
{
  Q_OBJECT
  Q_Q_CREATE(NXGroupBox)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
  explicit NXGroupBox(QWidget *parent = nullptr);
  explicit NXGroupBox(const QString& title, QWidget *parent = nullptr);
  ~NXGroupBox() override;

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXGROUPBOX_H
