#ifndef NXSPLASHSCREENPRIVATE_H
#define NXSPLASHSCREENPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "NXDef.h"
class NXText;
class NXProgressBar;
class NXProgressRing;
class NXSplashScreen;

class NXSplashScreenPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSplashScreen)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, Minimum)
  Q_PROPERTY_CREATE_D(int, Maximum)
  Q_PROPERTY_CREATE_D(int, Value)
  Q_PROPERTY_CREATE_D(bool, IsShowProgressBar)
  Q_PROPERTY_CREATE_D(bool, IsShowProgressRing)
  Q_PROPERTY_CREATE_D(bool, IsClosable)

public:
  explicit NXSplashScreenPrivate(QObject *parent = nullptr);
  ~NXSplashScreenPrivate();

  void _doShowAnimation();
  void _doCloseAnimation();

private:
  bool _isDragging { false };
  NXThemeType::ThemeMode _themeMode;
  QPoint _dragStartPos;
  QPixmap _logo;
  NXText *_titleText { nullptr };
  NXText *_subTitleText { nullptr };
  NXText *_statusText { nullptr };
  NXProgressBar *_progressBar { nullptr };
  NXProgressRing *_progressRing { nullptr };
};

#endif // NXSPLASHSCREENPRIVATE_H
