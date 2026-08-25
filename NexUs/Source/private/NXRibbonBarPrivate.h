#ifndef NXRIBBONBARPRIVATE_H
#define NXRIBBONBARPRIVATE_H

#include <QObject>
#include <QPointer>

#include "NXDef.h"
#include "NXProperty.h"

class QStackedWidget;
class QTimer;
class QVariantAnimation;
class NXRibbonBar;
class NXRibbonTabBar;

class NXRibbonBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXRibbonBar)
  Q_PROPERTY_CREATE_D(int, CurrentIndex)

public:
  explicit NXRibbonBarPrivate(QObject *parent = nullptr);
  ~NXRibbonBarPrivate();

  NXRibbonTabBar *activeTabBar() const;

private:
  bool _isCollapsed { false };
  bool _isPinned { false };
  NXThemeType::ThemeMode _themeMode;
  int _animationDuration { 200 };
  QPointer<NXRibbonTabBar> _externalTabBar;
  QStackedWidget *_stack { nullptr };
  NXRibbonTabBar *_internalTabBar { nullptr };
  QVariantAnimation *_heightAnim { nullptr };
  QTimer *_autoHideTimer { nullptr };
};

#endif // NXRIBBONBARPRIVATE_H
