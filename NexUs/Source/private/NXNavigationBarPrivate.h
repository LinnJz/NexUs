#ifndef NXNAVIGATIONBARPRIVATE_H
#define NXNAVIGATIONBARPRIVATE_H

#include <QObject>
#include <QVariantMap>

#include "NXDef.h"
#include "NXSuggestBox.h"
class QLayout;
class NXMenu;
class QVBoxLayout;
class QHBoxLayout;
class QLinearGradient;

class NXNavigationBar;
class NXNavigationNode;
class NXNavigationModel;
class NXNavigationView;
class NXNavigationStyle;
class NXInteractiveCard;

class NXBaseListView;
class NXFooterModel;
class NXFooterDelegate;
class NXIconButton;
class NXToolButton;

class NXNavigationBarPrivate : public QObject
{
  friend class NXNavigationView;
  friend class NXNavigationStyle;

  Q_OBJECT
  Q_D_CREATE(NXNavigationBar)
  Q_PROPERTY_CREATE(int, NavigationViewWidth);
  Q_PROPERTY_CREATE_D(int, NavigationBarWidth)
  Q_PROPERTY_CREATE(int, UserButtonSpacing);
  Q_PROPERTY_CREATE_D(bool, IsTransparent)
  Q_PROPERTY_CREATE_D(bool, IsAllowPageOpenInNewWindow)
public:
  explicit NXNavigationBarPrivate(QObject *parent = nullptr);
  ~NXNavigationBarPrivate() override;
  Q_SLOT void onNavigationButtonClicked();
  Q_SLOT void onNavigationOpenNewWindow(const QString& nodeKey);
  Q_SLOT void onNavigationCloseCurrentWindow(const QString& nodeKey);
  Q_INVOKABLE void onNavigationRoute(QVariantMap routeData);

  // 核心跳转逻辑
  void onTreeViewClicked(const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false);
  void onFooterViewClicked(const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false);

protected:
  virtual bool eventFilter(QObject *watched, QEvent *event);

private:
  bool _isShowUserCard { true };

  NXThemeType::ThemeMode _themeMode;
  NXNavigationType::NavigationDisplayMode _currentDisplayMode { NXNavigationType::NavigationDisplayMode::Maximal };
  QMap<QString, QString> _suggestKeyMap;
  QMap<QString, const QMetaObject *> _pageMetaMap;
  QMap<QString, int> _pageNewWindowCountMap;
  QMap<NXNavigationNode *, NXMenu *> _compactMenuMap;
  QList<NXSuggestBox::SuggestData> _suggestDataList;
  QList<NXNavigationNode *> _lastExpandedNodesList;
  std::function<void(const QString&)> _openPageFunc {};
  QVBoxLayout *_navigationButtonLayout { nullptr };
  QHBoxLayout *_navigationSuggestLayout { nullptr };
  QVBoxLayout *_userButtonLayout { nullptr };
  QVBoxLayout *_userCardLayout { nullptr };

  NXIconButton *_userButton { nullptr };
  NXToolButton *_searchButton { nullptr };
  NXToolButton *_navigationButton { nullptr };
  NXNavigationModel *_navigationModel { nullptr };
  NXNavigationView *_navigationView { nullptr };
  NXBaseListView *_footerView { nullptr };
  NXFooterModel *_footerModel { nullptr };
  NXFooterDelegate *_footerDelegate { nullptr };
  NXSuggestBox *_navigationSuggestBox { nullptr };
  NXInteractiveCard *_userCard { nullptr };

  void _initNodeModelIndex(const QModelIndex& parentIndex);
  void _resetNodeSelected();
  void _expandSelectedNodeParent();
  void _expandOrCollapseExpanderNode(NXNavigationNode *node, bool isExpand);

  void _addStackedPage(QWidget *page, QString pageKey);
  void _addFooterPage(QWidget *page, QString footKey);

  void _raiseNavigationBar();
  void _smoothScrollNavigationView(const QModelIndex& index);
  void _doComponentAnimation(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
  void _handleNavigationExpandState(bool isSave);
  // void _handleMaximalToCompactLayout();
  // void _handleCompactToMaximalLayout();
  void _resetLayout();

  void _doNavigationBarWidthAnimation(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
  void _doNavigationViewWidthAnimation(bool isAnimation);
  // void _doNavigationButtonAnimation(bool isCompact, bool isAnimation);
  // void _doSearchButtonAnimation(bool isCompact, bool isAnimation);
  void _doUserButtonAnimation(bool isCompact, bool isAnimation);
};

#endif // NXNAVIGATIONBARPRIVATE_H
