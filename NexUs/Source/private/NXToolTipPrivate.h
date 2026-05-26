#ifndef NXTOOLTIPPRIVATE_H
#define NXTOOLTIPPRIVATE_H

#include <QObject>
#include <QPoint>
#include <QTimer>

#include "NXDef.h"
class NXToolTip;
class QVBoxLayout;
class NXText;

class NXToolTipPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXToolTip)
  Q_PROPERTY_CREATE_D(QWidget *, CustomWidget)
  Q_PROPERTY_CREATE(qreal, Opacity)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, DisplayMsec)
  Q_PROPERTY_CREATE_D(int, ShowDelayMsec)
  Q_PROPERTY_CREATE_D(int, HideDelayMsec)
  Q_PROPERTY_CREATE_D(int, OffSetX)
  Q_PROPERTY_CREATE_D(int, OffSetY)
  Q_PROPERTY_CREATE_D(bool, IsMoveEnable)

public:
  explicit NXToolTipPrivate(QObject *parent = nullptr);
  ~NXToolTipPrivate();

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private Q_SLOTS:
  void onShowTimeout();
  void onHideTimeout();
  void onAutoHideTimeout();

private:
  NXThemeType::ThemeMode _themeMode;
  int _shadowBorderWidth { 6 };
  NXText *_toolTipText { nullptr };
  QVBoxLayout *_mainLayout { nullptr };

  QTimer *_showTimer;
  QTimer *_hideTimer;
  QTimer *_autoHideTimer;

  void _doShowAnimation();
  void _updatePos();
  void _stopAllTimers();
};

#endif // NXTOOLTIPPRIVATE_H
