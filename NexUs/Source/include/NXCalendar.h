#ifndef NXCALENDAR_H
#define NXCALENDAR_H

#include <QDate>
#include <QWidget>

#include "NXProperty.h"
class NXCalendarPrivate;

class NX_EXPORT NXCalendar : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXCalendar)
  Q_PROPERTY_CREATE_H(QDate, SelectedDate)
  Q_PROPERTY_CREATE_H(QDate, MinimumDate)
  Q_PROPERTY_CREATE_H(QDate, MaximumDate)
  Q_PROPERTY_CREATE_H(int, BorderRaiuds)

public:
  explicit NXCalendar(QWidget *parent = nullptr);
  ~NXCalendar();
  Q_SIGNAL void clicked(QDate date);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXCALENDAR_H
