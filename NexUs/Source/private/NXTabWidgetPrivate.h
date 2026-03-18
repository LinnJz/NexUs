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
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXDragMonitor))

private:
  explicit NXDragMonitor(QObject *parent = nullptr);
  ~NXDragMonitor() override;
};

class NXTabWidget;
class NXTabBar;

class NXTabWidgetPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTabWidget)
  Q_PROPERTY_CREATE_D(bool, IsTabTransparent)
  Q_PROPERTY_CREATE_D(bool, IsContainerAcceptDrops)
  Q_PROPERTY_CREATE_D(QSize, TabSize)

public:
  explicit NXTabWidgetPrivate(QObject *parent = nullptr);
  ~NXTabWidgetPrivate() override;
  Q_SLOT void onTabDragCreate(QMimeData *mimeData) noexcept;
  Q_SLOT void onTabDragEnter(QMimeData *mimeData) noexcept;
  Q_SLOT void onTabDragLeave(QMimeData *mimeData) noexcept;
  Q_SLOT void onTabDragDrop(QMimeData *mimeData) noexcept;
  Q_SLOT void onTabCloseRequested(int index) noexcept;

private:
  friend class NXCustomTabWidget;
  NXTabBar *_tabBar { nullptr };
  NXTabBar *_customTabBar { nullptr };
  QList<QWidget *> _allTabWidgetList;
  void _clearAllTabWidgetList() noexcept;
};

#endif // NXTABWIDGETPRIVATE_H
