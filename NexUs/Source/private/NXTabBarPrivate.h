#ifndef NXTABBARPRIVATE_H
#define NXTABBARPRIVATE_H

#include <QMimeData>
#include <QObject>
#include <QPixmap>

#include "NXProperty.h"
class NXTabBar;
class NXTabBarStyle;
class QTabBarPrivate;
class QPropertyAnimation;

class NXTabBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTabBar)
  Q_PROPERTY_CREATE(qreal, ScrollOffset)
  Q_PROPERTY_CREATE(qreal, TargetScrollOffset)

public:
  explicit NXTabBarPrivate(QObject *parent = nullptr);
  ~NXTabBarPrivate();

  void startScrollAnimation();
  void restoreScrollOffset(qreal offset);

private:
  QMimeData *_mimeData { nullptr };
  NXTabBarStyle *_style { nullptr };
  QTabBarPrivate *_tabBarPrivate { nullptr };
  QPropertyAnimation *_scrollAnimation { nullptr };
};

#endif // NXTABBARPRIVATE_H
