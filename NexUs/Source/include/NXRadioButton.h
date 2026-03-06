#ifndef NXRADIOBUTTON_H
#define NXRADIOBUTTON_H

#include <QRadioButton>

#include "NXDef.h"
class NXRadioButtonPrivate;

class NX_EXPORT NXRadioButton : public QRadioButton
{
  Q_OBJECT
  Q_Q_CREATE(NXRadioButton)
  Q_PROPERTY_CREATE_Q_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_Q_H(int, TextPointSize)
  Q_PROPERTY_CREATE_Q_H(NXTextType::TextStyle, TextStyle)

public:
  explicit NXRadioButton(QWidget *parent = nullptr);
  explicit NXRadioButton(const QString& text, QWidget *parent = nullptr);
  ~NXRadioButton() override;

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXRADIOBUTTON_H
