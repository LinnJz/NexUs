#ifndef NXSUGGESTBOXPRIVATE_H
#define NXSUGGESTBOXPRIVATE_H

#include <QAction>
#include <QObject>
#include <QSize>
#include <QVariantMap>
#include <QVector>

#include "NXDef.h"

class NXSuggestion : public QObject
{
  Q_OBJECT
  Q_PROPERTY_CREATE(QS_SET_CREF(QString), SuggestText)
  Q_PROPERTY_CREATE(QS_SET_CREF(QString), SuggestKey)
  Q_PROPERTY_CREATE(QS_SET_CREF(QVariantMap), SuggestData)
  Q_PROPERTY_CREATE(NXIconType::IconName, NXIcon)

public:
  explicit NXSuggestion(QObject *parent = nullptr);
  ~NXSuggestion();
};

class QVBoxLayout;
class NXLineEdit;
class NXNavigationNode;
class NXSuggestModel;
class NXBaseListView;
class NXSuggestDelegate;
class NXSuggestBox;
class NXSuggestBoxSearchViewContainer;

class NXSuggestBoxPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSuggestBox)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(Qt::CaseSensitivity, CaseSensitivity)

public:
  explicit NXSuggestBoxPrivate(QObject *parent = nullptr);
  ~NXSuggestBoxPrivate();
  Q_SLOT void onThemeModeChanged(NXThemeType::ThemeMode themeMode);
  Q_SLOT void onSearchEditTextEdit(const QString &searchText);
  Q_SLOT void onSearchViewClicked(const QModelIndex &index);

private:
  bool _isExpandAnimationFinished { true };
  bool _isCloseAnimationFinished { true };
  NXThemeType::ThemeMode _themeMode;
  QSize _lastSize;
  QVector<NXSuggestion *> _suggestionVector;
  QAction *_lightSearchAction { nullptr };
  QAction *_darkSearchAction { nullptr };
  NXSuggestBoxSearchViewContainer *_searchViewBaseWidget { nullptr };
  NXLineEdit *_searchEdit { nullptr };
  NXSuggestModel *_searchModel { nullptr };
  NXBaseListView *_searchView { nullptr };
  NXSuggestDelegate *_searchDelegate { nullptr };
  QVBoxLayout *_shadowLayout { nullptr };
  void _startSizeAnimation(QSize oldSize, QSize newSize);
  void _startExpandAnimation();
  void _startCloseAnimation();
};

#endif // NXSUGGESTBOXPRIVATE_H
