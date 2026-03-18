#ifndef NXPUSHBUTTON_H
#define NXPUSHBUTTON_H

#include <QPushButton>

#include "NXDef.h"
class NXPushButtonPrivate;

class NX_EXPORT NXPushButton : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXPushButton)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_H(int, TextPointSize)
  Q_PROPERTY_CREATE_H(NXTextType::TextStyle, TextStyle)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, LightDefaultColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, DarkDefaultColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, LightHoverColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, DarkHoverColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, LightPressColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, DarkPressColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, LightTextColor)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, DarkTextColor)

public:
  explicit NXPushButton(QWidget *parent = nullptr);
  explicit NXPushButton(const QString& text, QWidget *parent = nullptr);
  ~NXPushButton();

  void setNXIcon(NXIconType::IconName icon) noexcept;
  void setNXIcon(NXIconType::IconName icon, int iconSize) noexcept;

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXPUSHBUTTON_H
