#ifndef NXMULTISELECTCOMBOBOXDELEGATE_H
#define NXMULTISELECTCOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

#include "NXDef.h"

class NXMultiSelectComboBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  explicit NXMultiSelectComboBoxDelegate(QObject *parent = nullptr);
  ~NXMultiSelectComboBoxDelegate();

  void setItemSelection(const QVector<bool> &selection);

protected:
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
  NXThemeType::ThemeMode _themeMode;
  QVector<bool> _itemSelection;
};

#endif // NXMULTISELECTCOMBOBOXDELEGATE_H
