#ifndef NXSPINBOX_H
#define NXSPINBOX_H

#include <QSpinBox>

#include "NXDef.h"

class NXSpinBoxPrivate;

class NX_EXPORT NXSpinBox : public QSpinBox
{
  Q_OBJECT
  Q_Q_CREATE(NXSpinBox)
  Q_PROPERTY_CREATE_Q_H(NXSpinBoxType::ButtonMode, ButtonMode)
public:
  explicit NXSpinBox(QWidget *parent = nullptr);
  ~NXSpinBox() override;

protected:
  void focusInEvent(QFocusEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // NXSPINBOX_H
