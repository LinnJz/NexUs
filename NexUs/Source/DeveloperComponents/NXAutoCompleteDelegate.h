#ifndef NXUTOCOMPLETEDELEGATE_H
#define NXUTOCOMPLETEDELEGATE_H

#include <QStyledItemDelegate>

#include "NXAutoComplete.h"
#include "NXDef.h"

class NXAutoCompleteDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  explicit NXAutoCompleteDelegate(QObject *parent = nullptr);
  ~NXAutoCompleteDelegate();

  void setHighlightText(const QString &text);
  void setMatchMode(NXAutoComplete::MatchMode mode);

protected:
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
  NXThemeType::ThemeMode _themeMode;
  QString _highlightText;
  NXAutoComplete::MatchMode _matchMode { NXAutoComplete::Contains };
};

#endif // NXUTOCOMPLETEDELEGATE_H
