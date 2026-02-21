#ifndef NXTHEMEPRIVATE_H
#define NXTHEMEPRIVATE_H

#include <QColor>
#include <QMap>
#include <QObject>

#include "NXDef.h"
class NXTheme;

class NXThemePrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTheme)

public:
  explicit NXThemePrivate(QObject *parent = nullptr);
  ~NXThemePrivate();

private:
  NXThemeType::ThemeMode _themeMode { NXThemeType::Light };
  QColor _lightThemeColorList[42];
  QColor _darkThemeColorList[42];
  void _initThemeColor();
};

#endif // NXTHEMEPRIVATE_H
