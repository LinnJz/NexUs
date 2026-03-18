#ifndef NXCALENDARMODEL_H
#define NXCALENDARMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QMetaType>

#include "NXProperty.h"

enum NXCalendarType
{
  YearMode  = 0x00'01,
  MonthMode = 0x00'02,
  DayMode   = 0x00'03,
};

struct NXCalendarData : public QObjectData
{
public:
  NXCalendarData() { };
  ~NXCalendarData() { };
  NXCalendarData(int year, int month, int day, const QString& desText = {})
      : year(year)
      , month(month)
      , day(day)
      , desText(desText) { };

  NXCalendarData(const NXCalendarData& other)
  {
    year    = other.year;
    month   = other.month;
    day     = other.day;
    desText = other.desText;
  }

  int year  = 1924;
  int month = 1;
  int day   = 1;
  QString desText {};
};

Q_DECLARE_METATYPE(NXCalendarData);

class NXCalendarModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PRIVATE_CREATE_H(QDate, MinimumDate)
  Q_PRIVATE_CREATE_H(QDate, MaximumDate)
  Q_PRIVATE_CREATE_H(NXCalendarType, DisplayMode)

public:
  explicit NXCalendarModel(QObject *parent = nullptr);
  ~NXCalendarModel();

  QModelIndex getIndexFromDate(QDate date) const noexcept;
  QDate getDateFromIndex(const QModelIndex& index) const noexcept;
  QVariant data(const QModelIndex& index, int role) const override;

Q_SIGNALS:
  void currentYearMonthChanged(const QString& date);
  void displayModeChanged();

protected:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

private:
  NXCalendarType _displayMode { NXCalendarType::DayMode };
  int _dayRowCount { 0 };
  int _offset { 0 };
  QDate _pMinimumDate;
  QDate _pMaximumDate;

  void _initRowCount();
  int _getCurrentDay(int row) const;
};

#endif // NXCALENDARMODEL_H
