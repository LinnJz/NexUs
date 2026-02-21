#ifndef NXTOOLTIPPRIVATE_H
#define NXTOOLTIPPRIVATE_H

#include <QObject>
#include <QPoint>

#include "NXDef.h"
class QVBoxLayout;
class NXToolTip;
class NXText;

class NXToolTipPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXToolTip)
  Q_PROPERTY_CREATE_D(int, OffSetX)
  Q_PROPERTY_CREATE_D(int, OffSetY)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, DisplayMsec)
  Q_PROPERTY_CREATE_D(int, ShowDelayMsec)
  Q_PROPERTY_CREATE_D(int, HideDelayMsec)
  Q_PROPERTY_CREATE_D(QString, ToolTip)
  Q_PROPERTY_CREATE_D(QWidget *, CustomWidget)
  Q_PROPERTY_CREATE(qreal, Opacity)

public:
  explicit NXToolTipPrivate(QObject *parent = nullptr);
  ~NXToolTipPrivate() override;

protected:
  virtual bool eventFilter(QObject *watched, QEvent *event);

private:
  NXThemeType::ThemeMode _themeMode;
  int _shadowBorderWidth { 6 };
  QPoint _pos;
  NXText *_toolTipText { nullptr };
  QVBoxLayout *_mainLayout { nullptr };

  void _doShowAnimation();
  void _updatePos(const QPoint& pos);
};

#endif // NXTOOLTIPPRIVATE_H
