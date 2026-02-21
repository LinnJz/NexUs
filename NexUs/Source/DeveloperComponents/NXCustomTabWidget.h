#ifndef NXCUSTOMTABWIDGET_H
#define NXCUSTOMTABWIDGET_H

#include "NXCustomWidget.h"

class NXTabBar;
class NXTabWidget;

class NXCustomTabWidget : public NXCustomWidget
{
  Q_OBJECT
  Q_PROPERTY_CREATE(bool, IsFinished)
public:
  explicit NXCustomTabWidget(QWidget *parent = nullptr);
  ~NXCustomTabWidget() override;
  void addTab(QWidget *widget, QIcon tabIcon, const QString& tabTitle);
  NXTabBar *getCustomTabBar() const;
  NXTabWidget *getCustomTabWidget() const;

  Q_INVOKABLE bool processHitTest();

private:
  bool _isAllowLeave { false };
  NXTabBar *_customTabBar { nullptr };
  NXTabWidget *_customTabWidget { nullptr };
};

#endif // NXCUSTOMTABWIDGET_H
