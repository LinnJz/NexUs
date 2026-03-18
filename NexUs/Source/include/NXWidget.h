#ifndef NXWIDGET_H
#define NXWIDGET_H

#include <QWidget>
#include "NXAppBar.h"
class NXWidgetPrivate;

class NX_EXPORT NXWidget : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXWidget)
  Q_PROPERTY_CREATE_H(bool, IsStayTop)
  Q_PROPERTY_CREATE_H(bool, IsFixedSize)
  Q_PROPERTY_CREATE_H(bool, IsDefaultClosed)
  Q_PROPERTY_CREATE_H(int, AppBarHeight)
  Q_TAKEOVER_NATIVEEVENT_H

public:
  explicit NXWidget(QWidget *parent = nullptr);
  ~NXWidget() override;
  void moveToCenter() noexcept;

  void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true) noexcept;
  void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags) noexcept;
  NXAppBarType::ButtonFlags getWindowButtonFlags() const noexcept;

  NXAppBar *getAppBar() const noexcept;

Q_SIGNALS:
  void routeBackButtonClicked();
  void navigationButtonClicked();
  void themeChangeButtonClicked();
  void closeButtonClicked();

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXWIDGET_H
