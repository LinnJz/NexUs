#ifndef NXTABWIDGETPRIVATE_H
#define NXTABWIDGETPRIVATE_H

#include <QDrag>
#include <QObject>
#include <QPixmap>

#include "LinnSingleton.h"
#include "NXProperty.h"

class NXDragMonitor : public QObject
{
  Q_OBJECT
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXDragMonitor))
  Q_PRIVATE_CREATE(bool, IsInDrag)

private:
  explicit NXDragMonitor(QObject *parent = nullptr);
  ~NXDragMonitor();
};

class NXTabWidget;
class NXTabBar;

class NXTabWidgetPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTabWidget)
  Q_PROPERTY_CREATE_D(QSize, TabSize)
  Q_PROPERTY_CREATE_D(QSize, FloatWidgetSize)
  Q_PROPERTY_CREATE_D(bool, IsTabTransparent)
  Q_PROPERTY_CREATE_D(bool, IsContainerAcceptDrops)

public:
  explicit NXTabWidgetPrivate(QObject *parent = nullptr);
  ~NXTabWidgetPrivate();
  Q_SLOT void onTabDragCreate(QMimeData *mimeData);
  Q_SLOT void onTabDragEnter(QMimeData *mimeData);
  Q_SLOT void onTabDragLeave(QMimeData *mimeData);
  Q_SLOT void onTabDragDrop(QMimeData *mimeData);
  Q_SLOT void onTabCloseRequested(int index);

private:
  friend class NXCustomTabWidget;
  QList<QWidget *> _allTabWidgetList;
  NXTabBar *_tabBar { nullptr };
  NXTabBar *_customTabBar { nullptr };
  void _clearAllTabWidgetList();
};

#endif // NXTABWIDGETPRIVATE_H
