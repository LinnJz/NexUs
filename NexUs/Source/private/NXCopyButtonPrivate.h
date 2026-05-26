#ifndef NXCOPYBUTTONPRIVATE_H
#define NXCOPYBUTTONPRIVATE_H

#include <QColor>
#include <QObject>

#include "NXDef.h"

class QTimer;
class NXCopyButton;

class NXCopyButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXCopyButton)
  Q_PROPERTY_CREATE_D(QString, CopyText)
  Q_PROPERTY_CREATE_D(QString, SuccessText)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, SuccessDuration)

public:
  explicit NXCopyButtonPrivate(QObject *parent = nullptr);
  ~NXCopyButtonPrivate();

private:
  friend class NXCopyButton;
  bool _isPressed { false };
  bool _isSuccess { false };
  int _shadowBorderWidth { 3 };
  NXThemeType::ThemeMode _themeMode;
  NXIconType::IconName _normalIcon { NXIconType::Copy };
  NXIconType::IconName _successIcon { NXIconType::ClipboardCheck };
  qreal _iconOpacity { 1.0 };
  QTimer *_resetTimer { nullptr };
};

#endif // NXCOPYBUTTONPRIVATE_H
