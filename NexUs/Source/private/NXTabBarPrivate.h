#ifndef NXTABBARPRIVATE_H
#define NXTABBARPRIVATE_H

#include <QMimeData>
#include <QObject>
#include <QPixmap>
#include "NXProperty.h"
class NXTabBar;
class NXTabBarStyle;
class QTabBarPrivate;

class NXTabBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTabBar)

public:
  explicit NXTabBarPrivate(QObject *parent = nullptr);
  ~NXTabBarPrivate() override;

private:
  QMimeData *_mimeData { nullptr };
  NXTabBarStyle *_style { nullptr };
  QTabBarPrivate *_tabBarPrivate { nullptr };
};

#endif // NXTABBARPRIVATE_H
