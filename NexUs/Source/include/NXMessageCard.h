#ifndef NXMESSAGECARD_H
#define NXMESSAGECARD_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXMessageCardPrivate;

class NX_EXPORT NXMessageCard : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXMessageCard)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(bool, IsCompleted)
  Q_PROPERTY_CREATE_Q_H(QString, Title)
  Q_PROPERTY_CREATE_Q_H(QString, Content)
  Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize)
  Q_PROPERTY_CREATE_Q_H(int, ContentPixelSize)

public:
  explicit NXMessageCard(QWidget *parent = nullptr);
  ~NXMessageCard();

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXMESSAGECARD_H
