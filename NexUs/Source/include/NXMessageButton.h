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
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, DisplayMsec)
  Q_PROPERTY_CREATE_H(NXMessageBarType::MessageMode, MessageMode)
  Q_PROPERTY_CREATE_H(NXMessageBarType::PositionPolicy, PositionPolicy)
  Q_PROPERTY_CREATE_H(QWidget *, MessageTargetWidget)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, BarTitle)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, BarText)

public:
  explicit NXMessageButton(QWidget *parent = nullptr);
  explicit NXMessageButton(const QString& text, QWidget *parent = nullptr);
  ~NXMessageButton();
  void disconnectInternalSignalOfClicked() noexcept;
Q_SIGNALS:
  void showMessage();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXMESSAGEBUTTON_H
