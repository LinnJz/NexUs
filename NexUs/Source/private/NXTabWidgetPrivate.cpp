#include "NXTabWidgetPrivate.h"

#include <QApplication>
#include <QDebug>
#include <QMimeData>
#include <QMouseEvent>
#include <QTimer>
#include <QWindow>
#include "DeveloperComponents/NXCustomTabWidget.h"
#include "NXTabBar.h"
#include "NXTabWidget.h"

NXDragMonitor::NXDragMonitor(QObject *parent)
    : QObject(parent)
{
  _pIsInDrag = false;
}

NXDragMonitor::~NXDragMonitor() { }

NXTabWidgetPrivate::NXTabWidgetPrivate(QObject *parent)
    : QObject { parent }
{
}

NXTabWidgetPrivate::~NXTabWidgetPrivate() { }

void NXTabWidgetPrivate::onTabDragCreate(QMimeData *mimeData)
{
  Q_Q(NXTabWidget);
  if (NXDragMonitor::getInstance().getIsInDrag()) { return; }
  NXDragMonitor::getInstance().setIsInDrag(true);
  mimeData->setProperty("NXTabWidgetObject", QVariant::fromValue(q));
  int index           = q->currentIndex();
  QString tabText     = q->tabText(index);
  QIcon tabIcon       = q->tabIcon(index);
  QWidget *dragWidget = q->widget(index);
  dragWidget->setProperty("TabIcon", q->tabIcon(index));
  dragWidget->setProperty("TabText", q->tabText(index));
  QWidget *originTabWidget = dragWidget->property("NXOriginTabWidget").value<NXTabWidget *>();
  if (!originTabWidget && !dragWidget->property("IsMetaWidget").toBool())
  {
    dragWidget->setProperty("NXOriginTabWidget", QVariant::fromValue<NXTabWidget *>(q));
    originTabWidget = q;
  }
  mimeData->setProperty("DragWidget", QVariant::fromValue(dragWidget));
  NXTabBar *tabBarObject = mimeData->property("NXTabBarObject").value<NXTabBar *>();
  QMouseEvent releaseEvent(
      QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
  QApplication::sendEvent(tabBarObject, &releaseEvent);
  if (!originTabWidget) { originTabWidget = dragWidget->property("NXFloatParentWidget").value<QWidget *>(); }
  bool isFloatWidget             = mimeData->property("IsFloatWidget").toBool();
  QSize tabSize                  = mimeData->property("TabSize").toSize();
  NXCustomTabWidget *floatWidget = nullptr;
  if (isFloatWidget)
  {
    floatWidget = dynamic_cast<NXCustomTabWidget *>(q->window());
    if (floatWidget->windowHandle()) { floatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, true); }
  }
  else
  {
    floatWidget = new NXCustomTabWidget(originTabWidget);
    q->removeTab(index);
    // 创建新窗口
    NXTabBar *originCustomTabBar = tabBarObject;
    if (originCustomTabBar && originCustomTabBar->objectName() == "NXCustomTabBar")
    {
      originCustomTabBar->removeTab(index);
    }
    NXTabBar *customTabBar = floatWidget->getCustomTabBar();
    customTabBar->setTabSize(tabSize);
    dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar *>(customTabBar));
    floatWidget->addTab(dragWidget, tabIcon, tabText);
  }
  QPoint dragPos    = mimeData->property("DragPos").toPoint();
  QTimer *dragTimer = new QTimer(this);
  dragTimer->start(10);
  connect(dragTimer,
          &QTimer::timeout,
          floatWidget,
          [=]()
  {
    if (floatWidget->getIsFinished() && !isFloatWidget) { dragTimer->stop(); }
    else
    {
      QPoint cursorPoint = QCursor::pos();
      if (isFloatWidget) { floatWidget->move(cursorPoint.x() - dragPos.x() - 10, cursorPoint.y() - dragPos.y() - 10); }
      else
      {
        floatWidget->move(cursorPoint.x() - tabSize.width() / 2 - 10, cursorPoint.y() - tabSize.height() / 2 - 10);
      }
    }
  });
  QDrag *drag = new QDrag(this);
  QPixmap pix(1, 1);
  pix.fill(Qt::transparent);
  drag->setPixmap(pix);
  drag->setMimeData(mimeData);
  connect(drag, &QDrag::destroyed, this, [=]() { dragTimer->deleteLater(); });
  drag->setHotSpot(QPoint(tabSize.width() / 2, 0));
  QTimer::singleShot(1,
                     this,
                     [=]()
  {
    floatWidget->show();
    if (floatWidget->windowHandle()) { floatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, true); }
    if (!isFloatWidget) { floatWidget->resize(700, 500); }
  });
  auto ret = drag->exec();
  NXDragMonitor::getInstance().setIsInDrag(false);
  NXCustomTabWidget *tempFloatWidget = mimeData->property("TempFloatWidget").value<NXCustomTabWidget *>();
  if (tempFloatWidget)
  {
    if (ret == Qt::IgnoreAction)
    {
      if (tempFloatWidget->windowHandle())
      {
        tempFloatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, false);
      }
    }
    else
    {
      tempFloatWidget->deleteLater();
    }
    floatWidget->deleteLater();
  }
  else
  {
    if (floatWidget->windowHandle()) { floatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, false); }
  }
}

void NXTabWidgetPrivate::onTabDragEnter(QMimeData *mimeData)
{
  Q_Q(NXTabWidget);
  mimeData->setProperty("NXTabBarObject", QVariant::fromValue<NXTabBar *>(dynamic_cast<NXTabBar *>(q->tabBar())));
  onTabDragDrop(mimeData);
}

void NXTabWidgetPrivate::onTabDragLeave(QMimeData *mimeData)
{
  Q_Q(NXTabWidget);
  QWidget *dragWidget = mimeData->property("DragWidget").value<QWidget *>();
  int index           = q->indexOf(dragWidget);
  mimeData->setProperty("NXTabWidgetObject", QVariant::fromValue(q));
  QString tabText = q->tabText(index);
  QIcon tabIcon   = q->tabIcon(index);

  NXTabBar *tabBarObject = mimeData->property("NXTabBarObject").value<NXTabBar *>();
  mimeData->setProperty("DragWidget", QVariant::fromValue(dragWidget));
  q->removeTab(index);
  // 创建新窗口
  if (_customTabBar && _customTabBar != tabBarObject) { _customTabBar->removeTab(index); }
  QWidget *originTabWidget = dragWidget->property("NXOriginTabWidget").value<NXTabWidget *>();
  if (!originTabWidget) { originTabWidget = dragWidget->property("NXFloatParentWidget").value<QWidget *>(); }
  NXCustomTabWidget *floatWidget = new NXCustomTabWidget(originTabWidget);
  QSize tabSize                  = mimeData->property("TabSize").toSize();
  NXTabBar *customTabBar         = floatWidget->getCustomTabBar();
  customTabBar->setTabSize(tabSize);
  dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar *>(customTabBar));
  floatWidget->addTab(dragWidget, tabIcon, tabText);
  floatWidget->show();
  floatWidget->resize(700, 500);
  if (floatWidget->windowHandle()) { floatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, true); }
  QPoint cursorPoint = QCursor::pos();
  floatWidget->move(cursorPoint.x() - tabSize.width() / 2, cursorPoint.y() - tabSize.height() / 2);
  QTimer *dragTimer = new QTimer(this);
  dragTimer->start(10);
  connect(dragTimer,
          &QTimer::timeout,
          floatWidget,
          [=]()
  {
    QPoint cursorPoint = QCursor::pos();
    floatWidget->move(cursorPoint.x() - tabSize.width() / 2 - 10, cursorPoint.y() - tabSize.height() / 2 - 10);
  });
  connect(mimeData, &QMimeData::destroyed, this, [=]() { dragTimer->deleteLater(); });
  NXCustomTabWidget *tempFloatWidget = mimeData->property("TempFloatWidget").value<NXCustomTabWidget *>();
  if (tempFloatWidget) { tempFloatWidget->deleteLater(); }
  mimeData->setProperty("TempFloatWidget", QVariant::fromValue<NXCustomTabWidget *>(floatWidget));
}

void NXTabWidgetPrivate::onTabDragDrop(QMimeData *mimeData)
{
  Q_Q(NXTabWidget);
  QWidget *dragWidget = mimeData->property("DragWidget").value<QWidget *>();
  QString tabText     = dragWidget->property("TabText").toString();
  QIcon tabIcon       = dragWidget->property("TabIcon").value<QIcon>();
  int dropIndex       = mimeData->property("TabDropIndex").toInt();
  if (dropIndex < 0) { dropIndex = q->count(); }
  q->insertTab(dropIndex, dragWidget, tabIcon, tabText);
  q->setCurrentWidget(dragWidget);
  if (_customTabBar)
  {
    dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar *>(_customTabBar));
    _customTabBar->insertTab(dropIndex, tabIcon, tabText);
    _customTabBar->setCurrentIndex(dropIndex);
  }
}

void NXTabWidgetPrivate::onTabCloseRequested(int index)
{
  Q_Q(NXTabWidget);
  if (index < 0 || index >= q->count()) [[unlikely]] { return; }
  if (index == q->currentIndex())
  {
    int nextIndex = index > 0 ? index - 1 : (index + 1 < q->count() ? index + 1 : -1);
    if (nextIndex >= 0) { q->setCurrentIndex(nextIndex); }
  }
  _finalizeTabClose(index);
}

void NXTabWidgetPrivate::_finalizeTabClose(int index)
{
  Q_Q(NXTabWidget);
  QWidget *closeWidget         = q->widget(index);
  NXTabWidget *originTabWidget = closeWidget->property("NXOriginTabWidget").value<NXTabWidget *>();
  if (originTabWidget && originTabWidget != q)
  {
    NXTabBar *customTabBar = closeWidget->property("CurrentCustomBar").value<NXTabBar *>();
    if (customTabBar)
    {
      customTabBar->removeTab(index);
      closeWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar *>(nullptr));
    }
    originTabWidget->addTab(closeWidget, q->tabIcon(index), q->tabText(index));
    originTabWidget->setCurrentWidget(closeWidget);
  }
  else
  {
    if (!originTabWidget && q->objectName() == "NXCustomTabWidget") { _customTabBar->removeTab(index); }
    q->removeTab(index);
    if (_allTabWidgetList.contains(closeWidget)) { _allTabWidgetList.removeOne(closeWidget); }
    closeWidget->deleteLater();
  }
}

void NXTabWidgetPrivate::_clearAllTabWidgetList()
{
  Q_Q(NXTabWidget);
  for (auto widget : _allTabWidgetList)
  {
    auto originTabWidgetVariant = widget->property("NXOriginTabWidget");
    if (originTabWidgetVariant.isValid() && originTabWidgetVariant.value<NXTabWidget *>() == q)
    {
      widget->setProperty("NXOriginTabWidget", QVariant());
    }
  }
  _allTabWidgetList.clear();
}
