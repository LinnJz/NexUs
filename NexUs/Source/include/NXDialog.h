#ifndef NXFRAMEWORK_NXDIALOG_H
#define NXFRAMEWORK_NXDIALOG_H

#include <QDialog>

#include "NXAppBar.h"
#include "NXDef.h"

class NXDialogPrivate;

class NX_EXPORT NXDialog : public QDialog
{
  Q_OBJECT
  Q_Q_CREATE(NXDialog)
  Q_PROPERTY_CREATE_H(int, AppBarHeight)
  Q_PROPERTY_CREATE_H(bool, IsStayTop)
  Q_PROPERTY_CREATE_H(bool, IsFixedSize)
  Q_PROPERTY_CREATE_H(bool, IsDefaultClosed)
  Q_TAKEOVER_NATIVEEVENT_H

public:
  explicit NXDialog(QWidget *parent = nullptr);
  ~NXDialog();
  void moveToCenter();

  void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true);
  void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags);
  NXAppBarType::ButtonFlags getWindowButtonFlags() const;
  Q_SIGNAL void routeBackButtonClicked();
  Q_SIGNAL void navigationButtonClicked();
  Q_SIGNAL void themeChangeButtonClicked();
  Q_SIGNAL void closeButtonClicked();

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif //NXFRAMEWORK_NXDIALOG_H
