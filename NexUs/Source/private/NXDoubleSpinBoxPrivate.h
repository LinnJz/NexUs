#ifndef NXDOUBLESPINBOXPRIVATE_H
#define NXDOUBLESPINBOXPRIVATE_H

#include <QObject>

#include "NXDef.h"

class NXMenu;
class NXDoubleSpinBox;
class NXSpinBoxStyle;

class NXDoubleSpinBoxPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXDoubleSpinBox)
  Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)

public:
  explicit NXDoubleSpinBoxPrivate(QObject *parent = nullptr);
  ~NXDoubleSpinBoxPrivate();
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  NXThemeType::ThemeMode _themeMode;
  NXSpinBoxStyle *_style { nullptr };
  NXMenu *_createStandardContextMenu();
  void _changeTheme();
};

#endif // NXDOUBLESPINBOXPRIVATE_H
