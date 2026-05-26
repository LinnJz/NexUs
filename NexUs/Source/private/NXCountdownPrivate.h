#ifndef NXCOUNTDOWNPRIVATE_H
#define NXCOUNTDOWNPRIVATE_H

#include <QDateTime>
#include <QObject>

#include "NXDef.h"
class QTimer;
class NXCountdown;

class NXCountdownPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXCountdown)
  Q_PROPERTY_CREATE(qreal, FlipAngle)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, DigitWidth)
  Q_PROPERTY_CREATE_D(int, DigitHeight)
  Q_PROPERTY_CREATE_D(int, DigitSpacing)
  Q_PROPERTY_CREATE_D(int, FontPixelSize)
  Q_PROPERTY_CREATE_D(bool, IsShowDays)
  Q_PROPERTY_CREATE_D(bool, IsShowHours)
  Q_PROPERTY_CREATE_D(bool, IsShowMinutes)
  Q_PROPERTY_CREATE_D(bool, IsShowSeconds)

public:
  explicit NXCountdownPrivate(QObject *parent = nullptr);
  ~NXCountdownPrivate();

  Q_SLOT void onTimerTick();
  void _startFlipAnimation();

private:
  friend class NXCountdown;
  bool _isRunning { false };
  bool _useTargetMode { false };
  NXThemeType::ThemeMode _themeMode;
  int _prevSeconds { -1 };
  int _prevMinutes { -1 };
  int _prevHours { -1 };
  int _prevDays { -1 };
  qint64 _remainingSeconds { 0 };
  QDateTime _targetDateTime;
  QTimer *_timer { nullptr };
};

#endif // NXCOUNTDOWNPRIVATE_H
