#ifndef NXCONTENTDIALOG_H
#define NXCONTENTDIALOG_H
#include <QAbstractNativeEventFilter>
#include <QDialog>
#include <QString>

#include "NXAppBar.h"
#include "NXProperty.h"

class NXContentDialogPrivate;
class NXPushButton;

class NX_EXPORT NXContentDialog : public QDialog
{
  Q_OBJECT
  Q_Q_CREATE(NXContentDialog)
  Q_TAKEOVER_NATIVEEVENT_H

public:
  explicit NXContentDialog(QWidget *parent);
  ~NXContentDialog() override;

  void setLeftButtonText(const QString &text) noexcept;
  void setMiddleButtonText(const QString &text) noexcept;
  void setRightButtonText(const QString &text) noexcept;

  void setLeftButtonVisible(bool visible) noexcept;
  void setMiddleButtonVisible(bool visible) noexcept;
  void setRightButtonVisible(bool visible) noexcept;

  void setCentralWidget(QWidget *centralWidget);

  NXAppBar *appBar() const noexcept;

Q_SIGNALS:
  void leftButtonClicked();
  void middleButtonClicked();
  void rightButtonClicked();

protected:
  void showEvent(QShowEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
};

#endif // NXCONTENTDIALOG_H
