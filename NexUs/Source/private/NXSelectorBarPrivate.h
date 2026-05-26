#ifndef NXSELECTORBARPRIVATE_H
#define NXSELECTORBARPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXProperty.h"

class QVariantAnimation;
class NXSelectorBar;

class NXSelectorBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSelectorBar)
  Q_PROPERTY_CREATE_D(int, CurrentIndex)
  Q_PROPERTY_CREATE_D(int, BorderRadius)

public:
  struct SelectorItem
  {
    NXIconType::IconName icon = NXIconType::None;
    QString text;
  };

  explicit NXSelectorBarPrivate(QObject *parent = nullptr);
  ~NXSelectorBarPrivate();


private:
  NXThemeType::ThemeMode _themeMode;
  int _hoveredIndex { -1 };
  qreal _indicatorX { 0 };
  qreal _indicatorWidth { 0 };
  QList<SelectorItem> _items;
  QVariantAnimation *_indicatorAnimation { nullptr };
  void _startIndicatorAnimation(qreal targetX, qreal targetWidth);
  int _itemIndexAtPos(QPoint pos) const;
};

#endif // NXSELECTORBARPRIVATE_H
