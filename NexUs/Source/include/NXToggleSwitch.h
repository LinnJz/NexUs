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
  void setIsToggled(bool isToggled) noexcept;
  bool getIsToggled() const noexcept;

Q_SIGNALS:
  void toggled(bool checked);

protected:
  bool event(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXTOGGLESWITCH_H
