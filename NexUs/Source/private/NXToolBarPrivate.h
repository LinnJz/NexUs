#ifndef NXTOOLBARPRIVATE_H
#define NXTOOLBARPRIVATE_H
#include <QObject>

#include "NXDef.h"
class NXToolBarStyle;
class NXToolBar;

class NXToolBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXToolBar)

public:
  explicit NXToolBarPrivate(QObject *parent = nullptr);
  ~NXToolBarPrivate() override;

private:
  NXToolBarStyle *_toolBarStyle { nullptr };
  NXThemeType::ThemeMode _themeMode;
  int _shadowBorderWidth { 6 };
};

#endif // NXTOOLBARPRIVATE_H
