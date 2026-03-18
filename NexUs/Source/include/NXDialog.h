#ifndef NXFRAMEWORK_NXDIALOG_H
#define NXFRAMEWORK_NXDIALOG_H

#include <QDialog>
#include "NXAppBar.h"

class NXDialogPrivate;

class NX_EXPORT NXDialog : public QDialog
{
  Q_OBJECT
  Q_Q_CREATE(NXDialog)
  Q_PROPERTY_CREATE_H(bool, IsStayTop)
  Q_PROPERTY_CREATE_H(bool, IsFixedSize)
  Q_PROPERTY_CREATE_H(bool, IsDefaultClosed)
  Q_PROPERTY_CREATE_H(int, AppBarHeight)
  Q_TAKEOVER_NATIVEEVENT_H

public:
  explicit NXDialog(QWidget *parent = nullptr);
  ~NXDialog() override;
  void moveToCenter() noexcept;

  void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true) noexcept;
  void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags) noexcept;
  NXAppBarType::ButtonFlags getWindowButtonFlags() const noexcept;

Q_SIGNALS:
  void routeBackButtonClicked();
  void navigationButtonClicked();
  void themeChangeButtonClicked();
  void closeButtonClicked();

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXFRAMEWORK_NXDIALOG_H
