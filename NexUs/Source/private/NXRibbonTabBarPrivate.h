#ifndef NXRIBBONTABBARPRIVATE_H
#define NXRIBBONTABBARPRIVATE_H

#include <QObject>
#include <QRect>
#include <QStringList>

#include "NXDef.h"
#include "NXProperty.h"

class NXRibbonTabBar;

class NXRibbonTabBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXRibbonTabBar)
  Q_PROPERTY_CREATE_D(int, CurrentIndex)

public:
  explicit NXRibbonTabBarPrivate(QObject *parent = nullptr);
  ~NXRibbonTabBarPrivate();

  QList<QRect> tabRects() const;
  int tabAt(QPoint pos) const;

private:
  friend class NXRibbonTabBar;
  int _hoveredIndex { -1 };
  NXThemeType::ThemeMode _themeMode;
  QStringList _tabTitles;
};

#endif // NXRIBBONTABBARPRIVATE_H
