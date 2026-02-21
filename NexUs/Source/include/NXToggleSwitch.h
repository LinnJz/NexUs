#ifndef NXTOGGLESWITCH_H
#define NXTOGGLESWITCH_H

#include <QWidget>

#include "NXProperty.h"
class NXToggleSwitchPrivate;

class NX_EXPORT NXToggleSwitch : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXToggleSwitch);

public:
  explicit NXToggleSwitch(QWidget *parent = nullptr);
  ~NXToggleSwitch() override;
  void setIsToggled(bool isToggled);
  bool getIsToggled() const;
Q_SIGNALS:
  Q_SIGNAL void toggled(bool checked);

protected:
  virtual bool event(QEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXTOGGLESWITCH_H
