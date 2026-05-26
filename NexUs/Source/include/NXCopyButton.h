#ifndef NXCOPYBUTTONH
#define NXCOPYBUTTONH

#include <QPushButton>

#include "NXDef.h"
#include "NXProperty.h"
class NXCopyButtonPrivate;

class NX_EXPORT NXCopyButton : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXCopyButton)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), CopyText)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), SuccessText)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, SuccessDuration)

public:
  explicit NXCopyButton(QWidget *parent = nullptr);
  explicit NXCopyButton(const QString &text, QWidget *parent = nullptr);
  ~NXCopyButton();

  void setText(const QString &text);
  void setNXIcon(NXIconType::IconName icon);
  void setSuccessIcon(NXIconType::IconName icon);
  Q_SIGNAL void copyCompleted(const QString &text);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXCOPYBUTTONH
