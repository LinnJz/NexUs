#ifndef NXTABBARPRIVATE_H
#define NXTABBARPRIVATE_H

#include <QMimeData>
#include <QObject>
#include <QPixmap>
#include "NXProperty.h"

class NXTabBar;
class NXTabBarStyle;
class QTabBarPrivate;
class QHBoxLayout;
class QWidget;

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
  QWidget *_leftFixedContainer { nullptr };
  QWidget *_rightFixedContainer { nullptr };
  QHBoxLayout *_leftFixedLayout { nullptr };
  QHBoxLayout *_rightFixedLayout { nullptr };

  void _updateFixedAreaLayout();
};

#endif // NXTABBARPRIVATE_H
