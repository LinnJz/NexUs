#ifndef NXMESSAGEBUTTONPRIVATE_H
#define NXMESSAGEBUTTONPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXProperty.h"
class NXMessageButton;

class NXMessageButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXMessageButton)
  Q_PROPERTY_CREATE_D(QWidget *, MessageTargetWidget)
  Q_PROPERTY_CREATE_D(QString, BarTitle);
  Q_PROPERTY_CREATE_D(QString, BarText);
  Q_PROPERTY_CREATE_D(int, DisplayMsec);
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(NXMessageBarType::MessageMode, MessageMode);
  Q_PROPERTY_CREATE_D(NXMessageBarType::PositionPolicy, PositionPolicy);

public:
  explicit NXMessageButtonPrivate(QObject *parent = nullptr);
  ~NXMessageButtonPrivate();

private:
  bool _isLeftButtonPress { false };
  int _shadowBorderWidth { 3 };
  int _penBorderWidth { 1 };
  NXThemeType::ThemeMode _themeMode;

  Q_SLOT void _showMessage();
};

#endif // NXMESSAGEBUTTONPRIVATE_H
