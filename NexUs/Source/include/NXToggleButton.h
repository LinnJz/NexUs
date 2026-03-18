#ifndef NXTOGGLEBUTTON_H
#define NXTOGGLEBUTTON_H

#include <QPushButton>

#include "NXDef.h"
class NXToggleButtonPrivate;

class NX_EXPORT NXToggleButton : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXToggleButton)
  Q_PROPERTY_CREATE_H(bool, IsIconVisible)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, Text)

public:
  explicit NXToggleButton(QWidget *parent = nullptr);
  explicit NXToggleButton(const QString& text, QWidget *parent = nullptr);
  ~NXToggleButton() override;

  void setIsToggled(bool isToggled) noexcept;
  bool getIsToggled() const noexcept;

  void setNXIcon(NXIconType::IconName icon) noexcept;

Q_SIGNALS:
  void toggled(bool checked);

protected:
  bool event(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXTOGGLEBUTTON_H
