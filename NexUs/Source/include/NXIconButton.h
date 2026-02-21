#ifndef NXICONBUTTON_H
#define NXICONBUTTON_H

#include <QPushButton>

#include "NXDef.h"
#include "NXProperty.h"
class NXIconButtonPrivate;

class NX_EXPORT NXIconButton : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXIconButton)
  Q_PROPERTY_CREATE_Q_H(bool, IsSelected);
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(qreal, Opacity);
  Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor);
  Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor);
  Q_PROPERTY_CREATE_Q_H(QColor, LightIconColor);
  Q_PROPERTY_CREATE_Q_H(QColor, DarkIconColor);
  Q_PROPERTY_CREATE_Q_H(QColor, LightHoverIconColor);
  Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverIconColor);

public:
  NXIconButton(QPixmap pix, QWidget *parent = nullptr);
  NXIconButton(NXIconType::IconName awesome, QWidget *parent = nullptr);
  NXIconButton(NXIconType::IconName awesome, int pixelSize, QWidget *parent = nullptr);
  NXIconButton(NXIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget *parent = nullptr);
  ~NXIconButton();
  void setAwesome(NXIconType::IconName awesome);
  NXIconType::IconName getAwesome() const;

  void setPixmap(QPixmap pix);

protected:
  virtual bool event(QEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXICONBUTTON_H
