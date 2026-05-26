#ifndef NXPUSHBUTTON_H
#define NXPUSHBUTTON_H

#include <QPushButton>

#include "NXDef.h"
#include "NXProperty.h"
class NXPushButtonPrivate;

class NX_EXPORT NXPushButton : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXPushButton)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), LightDefaultColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), DarkDefaultColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), LightHoverColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), DarkHoverColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), LightPressColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), DarkPressColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), LightTextColor)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QColor), DarkTextColor)
  Q_PROPERTY_CREATE_H(NXTextType::TextStyle, TextStyle)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_H(int, TextPointSize)
  Q_PROPERTY_CREATE_H(bool, IsHoverEnable)

public:
  explicit NXPushButton(QWidget *parent = nullptr);
  explicit NXPushButton(const QString &text, QWidget *parent = nullptr);
  ~NXPushButton();

  void setNXIcon(NXIconType::IconName icon);
  void setNXIcon(NXIconType::IconName icon, int iconSize);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXPUSHBUTTON_H
