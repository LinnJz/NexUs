#ifndef NXCOLORDIALOG_H
#define NXCOLORDIALOG_H

#include <QDialog>

#include "NXAppBar.h"
class NXColorDialogPrivate;

class NX_EXPORT NXColorDialog : public QDialog
{
  Q_OBJECT
  Q_Q_CREATE(NXColorDialog)
  Q_PROPERTY_CREATE_Q_H(QColor, CurrentColor)
  Q_PROPERTY_CREATE_Q_H(NXColorSchemeType::ColorSchemeType, ColorSchemeType)
  Q_TAKEOVER_NATIVEEVENT_H
public:
  explicit NXColorDialog(QWidget *parent = nullptr);
  explicit NXColorDialog(const QColor& currentColor, QWidget *parent = nullptr);
  ~NXColorDialog() override;

  QList<QColor> getCustomColorList() const;
  QColor getCustomColor(int index) const;
  QString getCurrent4ChannelColor() const;
Q_SIGNALS:
  Q_SIGNAL void colorSelected(const QColor& color);

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXCOLORDIALOG_H
