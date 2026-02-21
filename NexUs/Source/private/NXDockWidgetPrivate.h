#ifndef NXDOCKWIDGETPRIVATE_H
#define NXDOCKWIDGETPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXDockWidget;
class NXDockWidgetTitleBar;

class NXDockWidgetPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXDockWidget)
public:
  explicit NXDockWidgetPrivate(QObject *parent = nullptr);
  ~NXDockWidgetPrivate();
  Q_SLOT void onThemeModeChanged(NXThemeType::ThemeMode themeMode);

private:
  qint64 _currentWinID { 0 };
  int _margins { 8 };
  int _shadowBorderWidth { 6 };
  NXThemeType::ThemeMode _themeMode;
  NXApplicationType::WindowDisplayMode _windowDisplayMode;
  NXDockWidgetTitleBar *_titleBar { nullptr };
};

#endif // NXDOCKWIDGETPRIVATE_H
