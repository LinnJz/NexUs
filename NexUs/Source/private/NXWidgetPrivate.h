#ifndef NXWIDGETPRIVATE_H
#define NXWIDGETPRIVATE_H

#include <QObject>
#include "NXDef.h"

class NXWidget;
class NXAppBar;

class NXWidgetPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXWidget)
public:
  explicit NXWidgetPrivate(QObject *parent = nullptr);
  ~NXWidgetPrivate() override;

private:
  NXApplicationType::WindowDisplayMode _windowDisplayMode;
  NXThemeType::ThemeMode _themeMode;
  NXAppBar *_appBar { nullptr };
};

#endif // NXWIDGETPRIVATE_H
