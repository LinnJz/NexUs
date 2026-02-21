#ifndef NXCALENDARPICKERCONTAINER_H
#define NXCALENDARPICKERCONTAINER_H

#include <QWidget>

#include "NXDef.h"

class NXCalendarPickerContainer : public QWidget
{
  Q_OBJECT
public:
  explicit NXCalendarPickerContainer(QWidget *parent = nullptr);
  ~NXCalendarPickerContainer();

protected:
  virtual void paintEvent(QPaintEvent *event) override;

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXCALENDARPICKERCONTAINER_H
