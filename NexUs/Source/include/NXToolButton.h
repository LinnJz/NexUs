#ifndef NXTOOLBUTTON_H
#define NXTOOLBUTTON_H

#include <QToolButton>

#include "NXDef.h"
class NXMenu;
class NXToolButtonPrivate;

class NX_EXPORT NXToolButton : public QToolButton
{
  Q_OBJECT
  Q_Q_CREATE(NXToolButton)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(bool, IsSelected)

public:
  explicit NXToolButton(QWidget *parent = nullptr);
  ~NXToolButton() override;

  void setIsTransparent(bool isTransparent) noexcept;
  bool getIsTransparent() const noexcept;

  void setMenu(NXMenu *menu) noexcept;
  void setNXIcon(NXIconType::IconName icon) noexcept;
  void setNXIcon(NXIconType::IconName icon, int rotate) noexcept;

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // NXTOOLBUTTON_H
