#ifndef NXCOLORPICKER_H
#define NXCOLORPICKER_H

#include <QPixmap>
#include <QWidget>

#include "NXDef.h"

class NXColorPicker : public QWidget
{
  Q_OBJECT
  Q_PRIVATE_CREATE(NXThemeType::ThemeMode, ThemeMode)

public:
  explicit NXColorPicker(QWidget *parent = nullptr);
  ~NXColorPicker();

  void setSelectedColor(QColor color);
  QColor getSelectedColor() const;

Q_SIGNALS:
  Q_SIGNAL void selectedColorChanged(QColor selectedColor);

protected:
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;

private:
  QImage _colorPickerImage;
  QPoint _selectedPoint;
  QColor _selectedColor;
  QPoint _adjustPointLimit(QPoint point);
};

#endif // NXCOLORPICKER_H
