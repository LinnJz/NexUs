#ifndef NXTABWIDGETPRIVATE_H
#define NXTABWIDGETPRIVATE_H

#include <QDrag>
#include <QObject>
#include <QPixmap>
#include "LinnSingleton.h"
#include "NXDef.h"

class NXDragMonitor : public QObject
{
  Q_OBJECT
  Q_PRIVATE_CREATE(bool, IsInDrag)
  LINN_SINGLETON_CREATE(LINN_SINGLETON_MEYERS(NXDragMonitor))

private:
  explicit NXDragMonitor(QObject *parent = nullptr);
  ~NXDragMonitor() override;
};

class NXTabWidget;
class NXTabBar;

class NXTabWidgetPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTabWidget);
  Q_PROPERTY_CREATE_D(QSize, TabSize)
  Q_PROPERTY_CREATE_D(bool, IsTabTransparent)
  Q_PROPERTY_CREATE_D(bool, IsContainerAcceptDrops)
  Q_PROPERTY_CREATE_D(bool, IsSelectedIndicatorVisible)
  Q_PROPERTY_CREATE_D(int, TabCornerRadius)
  Q_PROPERTY_CREATE_D(NXTabBarType::TabBarStyle, TabBarStyle)
public:
  explicit NXTabWidgetPrivate(QObject *parent = nullptr);
  ~NXTabWidgetPrivate() override;
  Q_SLOT void onTabDragCreate(QMimeData *mimeData);
  Q_SLOT void onTabDragEnter(QMimeData *mimeData);
  Q_SLOT void onTabDragLeave(QMimeData *mimeData);
  Q_SLOT void onTabDragDrop(QMimeData *mimeData);
  Q_SLOT void onTabCloseRequested(int index);

private:
  friend class NXCustomTabWidget;
  NXTabBar *_tabBar { nullptr };
  NXTabBar *_customTabBar { nullptr };
  QList<QWidget *> _allTabWidgetList;
  void _clearAllTabWidgetList();
  void _finalizeTabClose(int index);
};

#endif // NXTABWIDGETPRIVATE_H
