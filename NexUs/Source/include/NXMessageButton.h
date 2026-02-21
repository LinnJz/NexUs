#ifndef NXMESSAGEBUTTON_H
#define NXMESSAGEBUTTON_H

#include <QPushButton>
#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXMessageButtonPrivate;

class NX_EXPORT NXMessageButton : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXMessageButton)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, DisplayMsec);
  Q_PROPERTY_CREATE_Q_H(NXMessageBarType::MessageMode, MessageMode);
  Q_PROPERTY_CREATE_Q_H(NXMessageBarType::PositionPolicy, PositionPolicy);
  Q_PROPERTY_CREATE_Q_H(QString, BarTitle);
  Q_PROPERTY_CREATE_Q_H(QString, BarText);
  Q_PROPERTY_CREATE_Q_H(QWidget *, MessageTargetWidget)
public:
  explicit NXMessageButton(QWidget *parent = nullptr);
  explicit NXMessageButton(const QString& text, QWidget *parent = nullptr);
  ~NXMessageButton();
  void disconnectInternalSignalOfClicked();
Q_SIGNALS:
  Q_SIGNAL void showMessage();

protected:
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXMESSAGEBUTTON_H
