#ifndef NXCALENDARTITLEDELEGATE_H
#define NXCALENDARTITLEDELEGATE_H

#include <QStyledItemDelegate>

#include "NXDef.h"

class NXCalendarTitleDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit NXCalendarTitleDelegate(QObject *parent = nullptr);
  ~NXCalendarTitleDelegate();

  void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXCALENDARTITLEDELEGATE_H
