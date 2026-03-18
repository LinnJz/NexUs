#ifndef NXCOLORDIALOG_H
#define NXCOLORDIALOG_H

#include <QDialog>

#include "NXAppBar.h"
class NXColorDialogPrivate;

class NX_EXPORT NXColorDialog : public QDialog
{
  Q_OBJECT
  Q_Q_CREATE(NXColorDialog)
  Q_PROPERTY_CREATE_2_H(const QColor&, QColor, CurrentColor)
  Q_PROPERTY_CREATE_H(NXColorSchemeType::ColorSchemeType, ColorSchemeType)
  Q_TAKEOVER_NATIVEEVENT_H

public:
  explicit NXColorDialog(QWidget *parent = nullptr);
  explicit NXColorDialog(const QColor& currentColor, QWidget *parent = nullptr);
  ~NXColorDialog() override;

  QList<QColor> getCustomColorList() const noexcept;
  QColor getCustomColor(int index) const noexcept;
  QString getCurrent4ChannelColor() const noexcept;
Q_SIGNALS:
  void colorSelected(const QColor& color);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXCOLORDIALOG_H
