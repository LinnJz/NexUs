#ifndef NXPUSHBUTTON_H
#define NXPUSHBUTTON_H

#include <QPushButton>

#include "NXDef.h"
class NXPushButtonPrivate;

class NX_EXPORT NXPushButton : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXPushButton)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(QColor, LightDefaultColor)
  Q_PROPERTY_CREATE_Q_H(QColor, DarkDefaultColor)
  Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor)
  Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor)
  Q_PROPERTY_CREATE_Q_H(QColor, LightPressColor)
  Q_PROPERTY_CREATE_Q_H(QColor, DarkPressColor)
public:
  explicit NXPushButton(QWidget *parent = nullptr);
  explicit NXPushButton(const QString& text, QWidget *parent = nullptr);
  ~NXPushButton();

  void setLightTextColor(QColor color);
  QColor getLightTextColor() const;

  void setDarkTextColor(QColor color);
  QColor getDarkTextColor() const;

  void setTextStyle(NXTextType::TextStyle textStyle,
                    std::optional<int> pixelSize        = std::nullopt,
                    std::optional<QFont::Weight> weight = std::nullopt);
  void setNXIcon(NXIconType::IconName icon);
  void setNXIcon(NXIconType::IconName icon, int iconSize);

protected:
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXPUSHBUTTON_H
