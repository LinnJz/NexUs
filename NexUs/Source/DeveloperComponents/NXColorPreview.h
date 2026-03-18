#ifndef NXCOLORPREVIEW_H
#define NXCOLORPREVIEW_H

#include <QWidget>

#include "NXDef.h"

class NXColorPreview : public QWidget
{
  Q_OBJECT
  Q_PRIVATE_CREATE_2(const QColor&, QColor, BaseColor)
  Q_PRIVATE_CREATE(NXThemeType::ThemeMode, ThemeMode)

public:
  explicit NXColorPreview(QWidget *parent = nullptr);
  ~NXColorPreview();

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXCOLORPREVIEW_H
