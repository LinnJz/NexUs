#ifndef NXWINDOWPRIVATE_H
#define NXWINDOWPRIVATE_H

#include <QLinearGradient>
#include <QObject>
#include <QVariantMap>

#include "NXDef.h"
class NXEvent;
class NXWindow;
class NXNavigationBar;
class NXAppBar;
class NXCentralStackedWidget;
class NXThemeAnimationWidget;
class QHBoxLayout;
class QVBoxLayout;
class QMovie;

class NXWindowPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXWindow)
  Q_PROPERTY_CREATE_D(int, ThemeChangeTime)
  Q_PROPERTY_CREATE_D(NXWindowType::StackSwitchMode, StackSwitchMode)
  Q_PROPERTY_CREATE_D(NXNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
  Q_PROPERTY_CREATE_D(NXWindowType::PaintMode, WindowPaintMode)
public:
  explicit NXWindowPrivate(QObject *parent = nullptr);
  ~NXWindowPrivate() override;
  Q_SLOT void onNavigationButtonClicked();
  Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
  Q_SLOT void onThemeReadyChange();
  Q_SLOT void onThemeModeChanged(NXThemeType::ThemeMode themeMode);
  Q_SLOT void onWindowDisplayModeChanged();
  Q_SLOT void
  onNavigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey, bool isRouteBack);
  Q_SLOT void
  onNavigationNodeAdded(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey, QWidget *page);
  Q_SLOT void onNavigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey);
  Q_SLOT void onNavigationRouterStateChanged(NXNavigationRouterType::RouteMode routeMode);
  Q_INVOKABLE void onNavigationRoute(QVariantMap routeData);

private:
  bool _isNavigationBarFloat { false };
  bool _isWindowClosing { false };
  bool _isNavigationDisplayModeChanged { false };
  bool _isNavigationEnable { true };
  bool _isNavigationBarExpanded { false };
  bool _isInitFinished { false };
  int _contentsMargins { 5 };
  int _navigationTargetIndex { 0 };
  int _centralStackTargetIndex { 0 };
  NXNavigationType::NavigationDisplayMode _currentNavigationBarDisplayMode { NXNavigationType::Maximal };
  NXThemeType::ThemeMode _themeMode;
  NXApplicationType::WindowDisplayMode _windowDisplayMode;
  QString _lightWindowMoviePath { "" };
  QString _darkWindowMoviePath { "" };
  QPixmap *_lightWindowPix;
  QPixmap *_darkWindowPix;
  QMovie *_windowPaintMovie { nullptr };
  NXEvent *_focusEvent { nullptr };
  NXNavigationBar *_navigationBar { nullptr };
  NXCentralStackedWidget *_centerStackedWidget { nullptr };
  NXCentralStackedWidget *_navigationCenterStackedWidget { nullptr };
  NXAppBar *_appBar { nullptr };
  QHBoxLayout *_centerLayout { nullptr };
  NXThemeAnimationWidget *_animationWidget { nullptr };

  QMap<QString, const QMetaObject *> _pageMetaMap;
  QMap<QString, QWidget *> _routeMap; // key__nodeKey title可以一致  value__Page

  qreal _distance(QPoint point1, QPoint point2);
  void _resetWindowLayout(bool isAnimation);
  void _doNavigationDisplayModeChange();
};

#endif // NXWINDOWPRIVATE_H
