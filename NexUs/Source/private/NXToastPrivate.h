#ifndef NXTOASTPRIVATE_H
#define NXTOASTPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXToast.h"

class NXToastPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXToast)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, DisplayMsec)

public:
  explicit NXToastPrivate(QObject *parent = nullptr);
  ~NXToastPrivate();

private:
  NXThemeType::ThemeMode _themeMode;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
  int _shadowBorderWidth { 0 };
#else
  int _shadowBorderWidth { 6 };
#endif
  NXToast::ToastType _toastType { NXToast::Info };
  QString _text;
};

#endif // NXTOASTPRIVATE_H
