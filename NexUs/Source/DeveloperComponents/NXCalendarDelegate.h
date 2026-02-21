#ifndef NXCALENDARDELEGATE_H
#define NXCALENDARDELEGATE_H

#include <QDate>
#include <QStyledItemDelegate>

#include "NXCalendarModel.h"
#include "NXDef.h"

class NXCalendarDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  Q_PRIVATE_CREATE(int, ItemWidth)
  Q_PRIVATE_CREATE(int, ItemHeight)
  Q_PRIVATE_CREATE(bool, IsTransparent)
public:
  explicit NXCalendarDelegate(NXCalendarModel *calendarModel, QObject *parent = nullptr);
  ~NXCalendarDelegate();

  Q_SLOT void onCalendarModelDisplayModeChanged();

  void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
  NXThemeType::ThemeMode _themeMode;
  NXCalendarModel *_calendarModel { nullptr };
  QDate _nowDate;

  void _drawYearOrMonth(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
  void _drawDays(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // NXCALENDARDELEGATE_H
