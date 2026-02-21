#ifndef NXCONTENTDIALOG_H
#define NXCONTENTDIALOG_H
#include <QAbstractNativeEventFilter>
#include <QDialog>

#include "NXAppBar.h"
#include "NXProperty.h"

class NXContentDialogPrivate;

class NX_EXPORT NXContentDialog : public QDialog
{
  Q_OBJECT
  Q_Q_CREATE(NXContentDialog)
  Q_TAKEOVER_NATIVEEVENT_H

public:
  explicit NXContentDialog(QWidget *parent);
  ~NXContentDialog() override;
  Q_SLOT virtual void onLeftButtonClicked();
  Q_SLOT virtual void onMiddleButtonClicked();
  Q_SLOT virtual void onRightButtonClicked();
  void setCentralWidget(QWidget *centralWidget);

  void setLeftButtonText(const QString& text);
  void setMiddleButtonText(const QString& text);
  void setRightButtonText(const QString& text);

  void setLeftButtonVisible(bool visible);
  void setMiddleButtonVisible(bool visible);
  void setRightButtonVisible(bool visible);

  void close();
  NXAppBar *appBar() const;
Q_SIGNALS:
  Q_SIGNAL void leftButtonClicked();
  Q_SIGNAL void middleButtonClicked();
  Q_SIGNAL void rightButtonClicked();

protected:
  virtual void showEvent(QShowEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
  virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // NXCONTENTDIALOG_H
