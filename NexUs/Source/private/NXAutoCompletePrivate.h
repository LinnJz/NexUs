#ifndef NXUTOCOMPLETEPRIVATE_H
#define NXUTOCOMPLETEPRIVATE_H

#include <QObject>
#include <QStringList>

#include "NXAutoComplete.h"
#include "NXDef.h"

class NXLineEdit;
class NXBaseListView;
class QVBoxLayout;
class QStringListModel;
class QSortFilterProxyModel;
class NXAutoCompleteContainer;

class NXAutoCompletePrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXAutoComplete)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, MaxVisibleItems)
  Q_PROPERTY_CREATE_D(Qt::CaseSensitivity, CaseSensitivity)

public:
  explicit NXAutoCompletePrivate(QObject *parent = nullptr);
  ~NXAutoCompletePrivate();

  Q_SLOT void onThemeModeChanged(NXThemeType::ThemeMode themeMode);
  Q_SLOT void onTextEdited(const QString &text);
  Q_SLOT void onItemClicked(const QModelIndex &index);

private:
  bool _isAllowHidePopup { false };
  bool _isPopupVisible { false };
  NXThemeType::ThemeMode _themeMode;
  NXAutoComplete::MatchMode _matchMode { NXAutoComplete::Contains };
  QStringList _completions;
  NXLineEdit *_lineEdit { nullptr };
  NXAutoCompleteContainer *_popup { nullptr };
  NXBaseListView *_listView { nullptr };
  QVBoxLayout *_popupLayout { nullptr };
  QStringListModel *_sourceModel { nullptr };
  QSortFilterProxyModel *_filterModel { nullptr };

  void _showPopup();
  void _hidePopup();
  void _updateFilter(const QString &text);
};

#endif // NXUTOCOMPLETEPRIVATE_H
