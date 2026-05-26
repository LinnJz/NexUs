#ifndef NXTHEMEPRIVATE_H
#define NXTHEMEPRIVATE_H

#include <QColor>
#include <QMap>
#include <QMetaObject>
#include <QObject>

#include "NXDef.h"
class NXTheme;

class NXThemePrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTheme)
  Q_PROPERTY_CREATE_D(bool, IsFollowSystemTheme, false)

public:
  explicit NXThemePrivate(QObject *parent = nullptr);
  ~NXThemePrivate();

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  NXThemeType::ThemeMode _themeMode { NXThemeType::Light };
  QMetaObject::Connection _colorSchemeConnection;
  QColor _lightThemeColorList[48];
  QColor _darkThemeColorList[48];
  void _initThemeColor();

  NXThemeType::ThemeMode _detectSystemThemeMode() const;
  void _applySystemTheme();
};

#endif // NXTHEMEPRIVATE_H
