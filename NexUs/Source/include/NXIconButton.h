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
  Q_PROPERTY_CREATE_H(bool, IsSelected)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(qreal, Opacity)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, LightHoverColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, DarkHoverColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, LightIconColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, DarkIconColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, LightHoverIconColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, DarkHoverIconColor)

public:
  NXIconButton(const QPixmap& pix, QWidget *parent = nullptr);
  NXIconButton(NXIconType::IconName awesome, QWidget *parent = nullptr);
  NXIconButton(NXIconType::IconName awesome, int pixelSize, QWidget *parent = nullptr);
  NXIconButton(NXIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget *parent = nullptr);
  ~NXIconButton();
  void setAwesome(NXIconType::IconName awesome) noexcept;
  NXIconType::IconName getAwesome() const noexcept;

  void setPixmap(const QPixmap& pix) noexcept;

protected:
  bool event(QEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXICONBUTTON_H
