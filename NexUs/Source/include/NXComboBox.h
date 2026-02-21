#ifndef NXCOMBOBOX_H
#define NXCOMBOBOX_H

#include <QComboBox>

#include "NXProperty.h"

class NXComboBoxPrivate;

class NX_EXPORT NXComboBox : public QComboBox
{
  Q_OBJECT
  Q_Q_CREATE(NXComboBox);
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
  explicit NXComboBox(QWidget *parent = nullptr);
  ~NXComboBox() override;

  void setEditable(bool editable);

protected:
  virtual void showPopup() override;
  virtual void hidePopup() override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXCOMBOBOX_H
