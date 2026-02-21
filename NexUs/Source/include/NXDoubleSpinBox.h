#ifndef NXDOUBLESPINBOX_H
#define NXDOUBLESPINBOX_H

#include <QDoubleSpinBox>

#include "NXDef.h"

class NXDoubleSpinBoxPrivate;

class NX_EXPORT NXDoubleSpinBox : public QDoubleSpinBox
{
  Q_OBJECT
  Q_Q_CREATE(NXDoubleSpinBox)
  Q_PROPERTY_CREATE_Q_H(NXSpinBoxType::ButtonMode, ButtonMode)
public:
  explicit NXDoubleSpinBox(QWidget *parent = nullptr);
  ~NXDoubleSpinBox() override;

protected:
  void focusInEvent(QFocusEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // NXDOUBLESPINBOX_H
