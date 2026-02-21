#ifndef NXGROUPBOXPRIVATE_H
#define NXGROUPBOXPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXGroupBox;

class NXGroupBoxPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXGroupBox)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
  explicit NXGroupBoxPrivate(QObject *parent = nullptr);
  ~NXGroupBoxPrivate() override;
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXGROUPBOXPRIVATE_H
