#ifndef T_HOME_H
#define T_HOME_H

#include "T_BasePage.h"
class NXMenu;

class T_Home : public T_BasePage
{
  Q_OBJECT

public:
  Q_INVOKABLE explicit T_Home(QWidget *parent = nullptr);
  ~T_Home();
  Q_SIGNAL void screenNavigation();
  Q_SIGNAL void baseComponentNavigation();
  Q_SIGNAL void sceneNavigation();
  Q_SIGNAL void cardNavigation();
  Q_SIGNAL void iconNavigation();

protected:
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  NXMenu *_homeMenu { nullptr };
};

#endif // T_HOME_H
