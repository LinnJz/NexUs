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
  ~NXDoubleSpinBoxPrivate() override;
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  NXSpinBoxStyle *_style { nullptr };
  NXThemeType::ThemeMode _themeMode;
  NXMenu *_createStandardContextMenu();
  void _changnxTheme();
};

#endif // NXDOUBLESPINBOXPRIVATE_H
