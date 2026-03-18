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

  void setSelectedColor(const QColor& color) noexcept;
  QColor getSelectedColor() const noexcept;

Q_SIGNALS:
  void selectedColorChanged(QColor selectedColor);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  QImage _colorPickerImage;
  QPoint _selectedPoint;
  QColor _selectedColor;
  QPoint _adjustPointLimit(QPoint point);
};

#endif // NXCOLORPICKER_H
