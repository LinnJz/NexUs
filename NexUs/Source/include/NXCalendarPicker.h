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
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PRIVATE_CREATE_Q_H(QDate, SelectedDate)
public:
  explicit NXCalendarPicker(QWidget *parent = nullptr);
  ~NXCalendarPicker();

Q_SIGNALS:
  Q_SIGNAL void selectedDateChanged(QDate date);

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXCALENDARPICKER_H
