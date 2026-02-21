#ifndef NXWIDGET_H
#define NXWIDGET_H

#include <QWidget>
#include "NXAppBar.h"
class NXWidgetPrivate;

class NX_EXPORT NXWidget : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXWidget)
  Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
  Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
  Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
  Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
  Q_TAKEOVER_NATIVEEVENT_H
public:
  explicit NXWidget(QWidget *parent = nullptr);
  ~NXWidget() override;
  void moveToCenter();

  void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true);
  void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags);
  NXAppBarType::ButtonFlags getWindowButtonFlags() const;
  NXAppBar *appBar() const;
Q_SIGNALS:
  Q_SIGNAL void routeBackButtonClicked();
  Q_SIGNAL void navigationButtonClicked();
  Q_SIGNAL void themeChangeButtonClicked();
  Q_SIGNAL void closeButtonClicked();

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXWIDGET_H
