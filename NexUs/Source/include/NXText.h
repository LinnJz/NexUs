#ifndef NXTEXT_H
#define NXTEXT_H

#include <QLabel>

#include "NXDef.h"
class NXTextPrivate;

class NX_EXPORT NXText : public QLabel
{
  Q_OBJECT
  Q_Q_CREATE(NXText)
  Q_PROPERTY_CREATE_Q_H(bool, IsAllowClick)
  Q_PROPERTY_CREATE_Q_H(bool, IsWrapAnywhere)
  Q_PROPERTY_CREATE_Q_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_Q_H(int, TextPointSize)
  Q_PROPERTY_CREATE_Q_H(NXIconType::IconName, NXIcon)
  Q_PROPERTY_CREATE_Q_H(NXWidgetType::BorderFlags, BorderFlag)
public:
  explicit NXText(QWidget *parent = nullptr);
  explicit NXText(const QString& text, QWidget *parent = nullptr);
  explicit NXText(const QString& text, int pixelSize, QWidget *parent = nullptr);
  ~NXText() override;

  void setBorderStyle(int pixelSize, NXWidgetType::BorderFlags borderFlag, QColor color);
  void setTextStyle(NXTextType::TextStyle textStyle,
                    std::optional<int> pixelSize        = std::nullopt,
                    std::optional<QFont::Weight> weight = std::nullopt);
  NXTextType::TextStyle getTextStyle() const;
  Q_SIGNAL void pTextStyleChanged();
  Q_SIGNAL void clicked();

protected:
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void enterEvent(QEnterEvent *event) override;
  virtual void leaveEvent(QEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXTEXT_H
