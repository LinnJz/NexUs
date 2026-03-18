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
  enum ButtonType
  {
    LeftButton,
    MiddleButton,
    RightButton
  };
  Q_ENUM(ButtonType)

  explicit NXContentDialog(QWidget *parent);
  ~NXContentDialog() override;

  void setCentralWidget(QWidget *centralWidget) noexcept;

  NXPushButton *leftButton() const noexcept;
  NXPushButton *middleButton() const noexcept;
  NXPushButton *rightButton() const noexcept;
  NXPushButton *button(ButtonType button) const noexcept;

  void setButtonText(ButtonType button, const QString& text) noexcept;
  QString getButtonText(ButtonType button) const noexcept;

  void setIsButtonVisible(ButtonType button, bool visible) noexcept;
  bool getIsButtonVisible(ButtonType button) const noexcept;

  void setButtonDoneCode(ButtonType button, int doneCode) noexcept;
  int getButtonDoneCode(ButtonType button) const noexcept;

  void doneWithAnimation(int code) noexcept;

  NXAppBar *appBar() const noexcept;
Q_SIGNALS:
  void buttonClicked(ButtonType buttonType);

protected:
  void showEvent(QShowEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
};

#endif // NXCONTENTDIALOG_H
