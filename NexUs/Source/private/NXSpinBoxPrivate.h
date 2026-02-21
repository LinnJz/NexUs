#ifndef NXSPINBOXPRIVATE_H
#define NXSPINBOXPRIVATE_H

#include <QObject>

#include "NXDef.h"

class NXMenu;
class NXSpinBox;
class NXSpinBoxStyle;

class NXSpinBoxPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSpinBox)
  Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
  explicit NXSpinBoxPrivate(QObject *parent = nullptr);
  ~NXSpinBoxPrivate() override;
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  NXSpinBoxStyle *_style { nullptr };
  NXThemeType::ThemeMode _themeMode;
  NXMenu *_createStandardContextMenu();
};

#endif // NXSPINBOXPRIVATE_H
