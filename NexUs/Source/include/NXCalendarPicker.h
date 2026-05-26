#ifndef NXCALENDARPICKER_H
#define NXCALENDARPICKER_H

#include <QDate>
#include <QPushButton>

#include "NXProperty.h"
class NXCalendarPickerPrivate;

class NX_EXPORT NXCalendarPicker : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXCalendarPicker)
  Q_PRIVATE_CREATE_H(QDate, SelectedDate)
  Q_PROPERTY_CREATE_H(int, BorderRadius)

public:
  explicit NXCalendarPicker(QWidget *parent = nullptr);
  ~NXCalendarPicker();
  Q_SIGNAL void selectedDateChanged(QDate date);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXCALENDARPICKER_H
