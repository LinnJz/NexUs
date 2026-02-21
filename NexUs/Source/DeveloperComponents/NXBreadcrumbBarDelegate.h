#ifndef NXBREADCRUMBBARDELEGATE_H
#define NXBREADCRUMBBARDELEGATE_H

#include <QModelIndex>
#include <QStyledItemDelegate>

#include "NXDef.h"

class NXBreadcrumbBarDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  Q_PRIVATE_CREATE(QModelIndex, PressIndex)
public:
  explicit NXBreadcrumbBarDelegate(QObject *parent = nullptr);
  ~NXBreadcrumbBarDelegate();

protected:
  void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXBREADCRUMBBARDELEGATE_H
