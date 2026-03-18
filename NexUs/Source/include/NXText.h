#ifndef NXTEXT_H
#define NXTEXT_H

#include <QLabel>

#include "NXDef.h"
class NXTextPrivate;

class NX_EXPORT NXText : public QLabel
{
  Q_OBJECT
  Q_Q_CREATE(NXText)
  Q_PROPERTY_CREATE_H(bool, IsAllowClick)
  Q_PROPERTY_CREATE_H(bool, IsWrapAnywhere)
  Q_PROPERTY_CREATE_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_H(int, TextPointSize)
  Q_PROPERTY_CREATE_H(NXTextType::TextStyle, TextStyle)
  Q_PROPERTY_CREATE_H(NXIconType::IconName, NXIcon)
  Q_PROPERTY_CREATE_H(NXTextType::DisplayMode, DisplayMode)

public:
  explicit NXText(QWidget *parent = nullptr);
  explicit NXText(const QString& text, QWidget *parent = nullptr);
  explicit NXText(const QString& text, int pixelSize, QWidget *parent = nullptr);
  ~NXText() override;

  Q_SIGNAL void clicked();

protected:
  void mouseReleaseEvent(QMouseEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXTEXT_H
