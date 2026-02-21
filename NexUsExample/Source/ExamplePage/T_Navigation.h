#ifndef T_NAVIGATION_H
#define T_NAVIGATION_H

#include "T_BasePage.h"

class NXPivot;
class NXTabWidget;
class NXBreadcrumbBar;

class T_Navigation : public T_BasePage
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit T_Navigation(QWidget *parent = nullptr);
  ~T_Navigation();

private:
  NXBreadcrumbBar *_breadcrumbBar { nullptr };
  NXPivot *_pivot { nullptr };
  NXTabWidget *_tabWidget { nullptr };
};

#endif // T_NAVIGATION_H
