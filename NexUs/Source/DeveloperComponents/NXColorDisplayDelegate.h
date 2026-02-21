#ifndef NXCOLORDISPLAYDELEGATE_H
#define NXCOLORDISPLAYDELEGATE_H

#include <QStyledItemDelegate>

#include "NXDef.h"

class NXColorDisplayDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  Q_PRIVATE_CREATE(NXThemeType::ThemeMode, ThemeMode)
public:
  explicit NXColorDisplayDelegate(QObject *parent = nullptr);
  ~NXColorDisplayDelegate();

  void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // NXCOLORDISPLAYDELEGATE_H
