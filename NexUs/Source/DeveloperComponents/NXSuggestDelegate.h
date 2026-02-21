#ifndef NXSUGGESTDELEGATE_H
#define NXSUGGESTDELEGATE_H

#include <QStyledItemDelegate>

#include "NXDef.h"

class NXSuggestDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit NXSuggestDelegate(QObject *parent = nullptr);
  ~NXSuggestDelegate();

protected:
  void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXSUGGESTDELEGATE_H
