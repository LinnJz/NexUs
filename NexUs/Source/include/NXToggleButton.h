#ifndef NXTOGGLEBUTTON_H
#define NXTOGGLEBUTTON_H

#include <QPushButton>

#include "NXDef.h"
class NXToggleButtonPrivate;

class NX_EXPORT NXToggleButton : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXToggleButton)
  Q_PROPERTY_CREATE_Q_H(bool, IsIconVisible)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(QString, Text)
public:
  explicit NXToggleButton(QWidget *parent = nullptr);
  explicit NXToggleButton(QString text, QWidget *parent = nullptr);
  ~NXToggleButton() override;

  void setIsToggled(bool isToggled);
  bool getIsToggled() const;

  void setNXIcon(NXIconType::IconName icon);
Q_SIGNALS:
  Q_SIGNAL void toggled(bool checked);

protected:
  virtual bool event(QEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXTOGGLEBUTTON_H
