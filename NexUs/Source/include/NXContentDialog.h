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

  void setCentralWidget(QWidget *centralWidget);

  NXPushButton *leftButton() const;
  NXPushButton *middleButton() const;
  NXPushButton *rightButton() const;
  NXPushButton *button(ButtonType button) const;

  void setButtonText(ButtonType button, const QString& text);
  QString getButtonText(ButtonType button) const;

  void setIsButtonVisible(ButtonType button, bool visible);
  bool getIsButtonVisible(ButtonType button) const;

  void setButtonDoneCode(ButtonType button, int doneCode);
  int getButtonDoneCode(ButtonType button) const;

  void doneWithAnimation(int code);

  NXAppBar *appBar() const;
Q_SIGNALS:
  Q_SIGNAL void buttonClicked(ButtonType buttonType);

protected:
  virtual void showEvent(QShowEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
  virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // NXCONTENTDIALOG_H
