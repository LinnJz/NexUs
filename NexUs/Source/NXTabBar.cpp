#include "NXTabBar.h"

#include <QApplication>
#include <QDrag>
#include <QEasingCurve>
#include <QHBoxLayout>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QVariantAnimation>
#include <private/qtabbar_p.h>

#include "DeveloperComponents/NXTabBarStyle.h"
#include "NXTheme.h"
#include "private/NXTabBarPrivate.h"

#include <QTimer>

NXTabBar::NXTabBar(QWidget *parent)
    : QTabBar(parent)
    , d_ptr(new NXTabBarPrivate())
{
  Q_D(NXTabBar);
  d->q_ptr = this;
  setObjectName("NXTabBar");
  setMouseTracking(true);
  setStyleSheet("#NXTabBar{background-color:transparent;}");
  setTabsClosable(true);
  setMovable(true);
  setAcceptDrops(true);
  d->_style = new NXTabBarStyle(style());
  setStyle(d->_style);
  d->_tabBarPrivate       = dynamic_cast<QTabBarPrivate *>(this->QTabBar::d_ptr.data());
  d->_leftFixedContainer  = new QWidget(this);
  d->_rightFixedContainer = new QWidget(this);
  d->_leftFixedLayout     = new QHBoxLayout(d->_leftFixedContainer);
  d->_leftFixedLayout->setContentsMargins(0, 0, 0, 0);
  d->_leftFixedLayout->setSpacing(6);
  d->_leftFixedLayout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
  d->_rightFixedLayout = new QHBoxLayout(d->_rightFixedContainer);
  d->_rightFixedLayout->setContentsMargins(0, 0, 0, 0);
  d->_rightFixedLayout->setSpacing(6);
  d->_rightFixedLayout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  d->_leftFixedContainer->setVisible(false);
  d->_rightFixedContainer->setVisible(false);
}

NXTabBar::~NXTabBar()
{
  Q_D(NXTabBar);
  delete d->_style;
}

void NXTabBar::setTabSize(QSize tabSize)
{
  Q_D(NXTabBar);
  d->_style->setTabSize(tabSize);
  update();
}

QSize NXTabBar::getTabSize() const
{
  Q_D(const NXTabBar);
  return d->_style->getTabSize();
}

void NXTabBar::setTabBarStyle(NXTabBarType::TabBarStyle style)
{
  Q_D(NXTabBar);
  d->_style->setTabBarStyle(style);
  update();
}

NXTabBarType::TabBarStyle NXTabBar::getTabBarStyle() const
{
  Q_D(const NXTabBar);
  return d->_style->getTabBarStyle();
}

void NXTabBar::setTabCornerRadius(int radius)
{
  Q_D(NXTabBar);
  d->_style->setTabCornerRadius(radius);
  update();
}

int NXTabBar::getTabCornerRadius() const
{
  Q_D(const NXTabBar);
  return d->_style->getTabCornerRadius();
}

void NXTabBar::setIsSelectedIndicatorVisible(bool isVisible)
{
  Q_D(NXTabBar);
  d->_style->setIsSelectedIndicatorVisible(isVisible);
  update();
}

bool NXTabBar::getIsSelectedIndicatorVisible() const
{
  Q_D(const NXTabBar);
  return d->_style->getIsSelectedIndicatorVisible();
}

void NXTabBar::addLeftFixedButton(QWidget *widget)
{
  Q_D(NXTabBar);
  if (!widget || !d->_leftFixedLayout) { return; }
  widget->setParent(d->_leftFixedContainer);
  d->_leftFixedLayout->addWidget(widget);
  d->_updateFixedAreaLayout();
  update();
}

void NXTabBar::addRightFixedButton(QWidget *widget)
{
  Q_D(NXTabBar);
  if (!widget || !d->_rightFixedLayout) { return; }
  widget->setParent(d->_rightFixedContainer);
  d->_rightFixedLayout->addWidget(widget);
  d->_updateFixedAreaLayout();
  update();
}

void NXTabBar::removeLeftFixedButton(QWidget *widget)
{
  Q_D(NXTabBar);
  if (!widget || !d->_leftFixedLayout) { return; }
  d->_leftFixedLayout->removeWidget(widget);
  widget->setParent(nullptr);
  d->_updateFixedAreaLayout();
  update();
}

void NXTabBar::removeRightFixedButton(QWidget *widget)
{
  Q_D(NXTabBar);
  if (!widget || !d->_rightFixedLayout) { return; }
  d->_rightFixedLayout->removeWidget(widget);
  widget->setParent(nullptr);
  d->_updateFixedAreaLayout();
  update();
}

void NXTabBar::clearLeftFixedButtons()
{
  Q_D(NXTabBar);
  if (!d->_leftFixedLayout) { return; }
  while (d->_leftFixedLayout->count() > 0)
  {
    QLayoutItem *item = d->_leftFixedLayout->takeAt(0);
    if (item && item->widget()) { item->widget()->setParent(nullptr); }
    delete item, item = nullptr;
  }
  d->_updateFixedAreaLayout();
  update();
}

void NXTabBar::clearRightFixedButtons()
{
  Q_D(NXTabBar);
  if (!d->_rightFixedLayout) { return; }
  while (d->_rightFixedLayout->count() > 0)
  {
    QLayoutItem *item = d->_rightFixedLayout->takeAt(0);
    if (item && item->widget()) { item->widget()->setParent(nullptr); }
    delete item, item = nullptr;
  }
  d->_updateFixedAreaLayout();
  update();
}

QSize NXTabBar::sizeHint() const
{
  QSize oldSize = QTabBar::sizeHint();
  QSize newSize = oldSize;
  newSize.setWidth(parentWidget()->maximumWidth());
  return oldSize.expandedTo(newSize);
}

void NXTabBar::tabInserted(int index)
{
  Q_D(NXTabBar);
  QTabBar::tabInserted(index);
}

void NXTabBar::tabRemoved(int index)
{
  Q_D(NXTabBar);
  QTabBar::tabRemoved(index);
}

void NXTabBar::tabLayoutChange()
{
  Q_D(NXTabBar);
  QTabBar::tabLayoutChange();
}

void NXTabBar::mouseMoveEvent(QMouseEvent *event)
{
  QTabBar::mouseMoveEvent(event);
  Q_D(NXTabBar);
  if (d->_tabBarPrivate->pressedIndex >= 0)
  {
    QPoint currentPos = event->pos();
    if (objectName() == "NXCustomTabBar" && count() == 1)
    {
      if (!d->_mimeData)
      {
        d->_mimeData = new QMimeData();
        d->_mimeData->setProperty("DragType", "NXTabBarDrag");
        d->_mimeData->setProperty("NXTabBarObject", QVariant::fromValue(this));
        d->_mimeData->setProperty("TabSize", d->_style->getTabSize());
        d->_mimeData->setProperty("IsFloatWidget", true);
        QRect currentTabRect = tabRect(currentIndex());
        d->_mimeData->setProperty("DragPos",
                                  QPoint(currentPos.x() - currentTabRect.x(), currentPos.y() - currentTabRect.y()));
        Q_EMIT tabDragCreate(d->_mimeData);
        d->_mimeData = nullptr;
      }
    }
    else
    {
      auto& pressTabData = d->_tabBarPrivate->tabList[d->_tabBarPrivate->pressedIndex];
      QRect firstTabRect = tabRect(0);
#if (QT_VERSION > QT_VERSION_CHECK(6, 0, 0))
      QRect pressTabRect = pressTabData->rect;
      if (pressTabRect.right() + pressTabData->dragOffset > width() - firstTabRect.x())
      {
        pressTabData->dragOffset = width() - pressTabRect.right() - firstTabRect.x();
      }
      if (pressTabRect.x() + pressTabData->dragOffset < -firstTabRect.x())
      {
        pressTabData->dragOffset = -pressTabRect.x() - firstTabRect.x();
      }
#else
      QRect pressTabRect = pressTabData.rect;
      if (pressTabRect.right() + pressTabData.dragOffset > width() - firstTabRect.x())
      {
        pressTabData.dragOffset = width() - pressTabRect.right() - firstTabRect.x();
      }
      if (pressTabRect.x() + pressTabData.dragOffset < -firstTabRect.x())
      {
        pressTabData.dragOffset = -pressTabRect.x() - firstTabRect.x();
      }
#endif

      QRect moveRect = rect();
      moveRect.adjust(0, -height(), 0, height());
      if (currentPos.x() < 0 || currentPos.x() > width() || currentPos.y() > moveRect.bottom() ||
          currentPos.y() < moveRect.y())
      {
        if (!d->_mimeData)
        {
          d->_mimeData = new QMimeData();
          d->_mimeData->setProperty("DragType", "NXTabBarDrag");
          d->_mimeData->setProperty("NXTabBarObject", QVariant::fromValue(this));
          d->_mimeData->setProperty("TabSize", d->_style->getTabSize());
          Q_EMIT tabDragCreate(d->_mimeData);
          d->_mimeData = nullptr;
        }
      }
    }
  }
}

void NXTabBar::dragEnterEvent(QDragEnterEvent *event)
{
  Q_D(NXTabBar);
  if (event->mimeData()->property("DragType").toString() == "NXTabBarDrag")
  {
    event->acceptProposedAction();
    auto mimeData = const_cast<QMimeData *>(event->mimeData());
    d->_mimeData  = mimeData;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mimeData->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
    mimeData->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
    Q_EMIT tabDragEnter(mimeData);
    qApp->processEvents();
    QPoint localPressPos(tabRect(currentIndex()).x() + d->_style->getTabSize().width() / 2, 0);
    QPoint globalPressPos = mapToGlobal(localPressPos);

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    QMouseEvent pressEvent(QEvent::MouseButtonPress,
                           QPointF(localPressPos),
                           QPointF(globalPressPos),
                           Qt::LeftButton,
                           Qt::LeftButton,
                           Qt::NoModifier);
#else
    QMouseEvent pressEvent(QEvent::MouseButtonPress, localPressPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#endif
    QApplication::sendEvent(this, &pressEvent);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPointF localMovePosF(event->position().x(), 0);
    QPoint localMovePos  = localMovePosF.toPoint();
    QPoint globalMovePos = mapToGlobal(localMovePos);
#else
    QPoint localMovePos(event->pos().x(), 0);
    QPoint globalMovePos = mapToGlobal(localMovePos);
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    QMouseEvent moveEvent(QEvent::MouseMove,
                          QPointF(localMovePos),
                          QPointF(globalMovePos),
                          Qt::LeftButton,
                          Qt::LeftButton,
                          Qt::NoModifier);
#else
    QMouseEvent moveEvent(QEvent::MouseMove, localMovePos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#endif
    QApplication::sendEvent(this, &moveEvent);
  }
  QTabBar::dragEnterEvent(event);
}

void NXTabBar::dragMoveEvent(QDragMoveEvent *event)
{
  Q_D(NXTabBar);
  if (event->mimeData()->property("DragType").toString() == "NXTabBarDrag")
  {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPoint movePos(event->position().toPoint().x(), 0);
    QMouseEvent moveEvent(
        QEvent::MouseMove, movePos, mapToGlobal(movePos), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#else
    QMouseEvent moveEvent(
        QEvent::MouseMove, QPoint(event->pos().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#endif
    QApplication::sendEvent(this, &moveEvent);
  }
  QWidget::dragMoveEvent(event);
}

void NXTabBar::dragLeaveEvent(QDragLeaveEvent *event)
{
  Q_D(NXTabBar);
  if (d->_mimeData)
  {
    Q_EMIT tabDragLeave(d->_mimeData);
    d->_mimeData = nullptr;
  }
  QMouseEvent releaseEvent(
      QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
  QApplication::sendEvent(this, &releaseEvent);
  QTabBar::dragLeaveEvent(event);
}

void NXTabBar::dropEvent(QDropEvent *event)
{
  Q_D(NXTabBar);
  d->_mimeData = nullptr;
  QMouseEvent releaseEvent(
      QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
  QApplication::sendEvent(this, &releaseEvent);
  if (objectName() != "NXCustomTabBar")
  {
    QMimeData *data = const_cast<QMimeData *>(event->mimeData());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    data->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
    data->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
    Q_EMIT tabDragDrop(data);
  }
  QTabBar::dropEvent(event);
}

void NXTabBar::wheelEvent(QWheelEvent *event)
{
  QTabBar::wheelEvent(event);
  event->accept();
}

void NXTabBar::paintEvent(QPaintEvent *event)
{
  Q_D(NXTabBar);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(NXThemeColor(nxTheme->getThemeMode(), TabBarBase));
  painter.drawRect(rect());

  QSize tabSize = d->_style->getTabSize();
  for (int i = 0; i < d->_tabBarPrivate->tabList.size(); i++)
  {
#if (QT_VERSION > QT_VERSION_CHECK(6, 0, 0))
    d->_tabBarPrivate->tabList[i]->rect =
        QRect(tabSize.width() * i, d->_tabBarPrivate->tabList[i]->rect.y(), tabSize.width(), tabSize.height());
#else
    d->_tabBarPrivate->tabList[i].rect =
        QRect(tabSize.width() * i, d->_tabBarPrivate->tabList[i].rect.y(), tabSize.width(), tabSize.height());
#endif
  }
  d->_tabBarPrivate->layoutWidgets();
  QTabBar::paintEvent(event);

  constexpr int separatorSpace = 10;
  if (d->_leftFixedContainer && d->_leftFixedContainer->isVisible())
  {
    int leftWidth = d->_leftFixedContainer->width();
    QRectF indicatorRect(leftWidth + (separatorSpace - 3) / 2, 7, 3, height() - 14);
    painter.setBrush(NXThemeColor(nxTheme->getThemeMode(), PrimaryNormal));
    painter.drawRoundedRect(indicatorRect, 2, 2);
  }
  if (d->_rightFixedContainer && d->_rightFixedContainer->isVisible())
  {
    int rightWidth = d->_rightFixedContainer->width();
    QRectF indicatorRect(width() - rightWidth - separatorSpace + (separatorSpace - 3) / 2, 7, 3, height() - 14);
    painter.setBrush(NXThemeColor(nxTheme->getThemeMode(), PrimaryNormal));
    painter.drawRoundedRect(indicatorRect, 2, 2);
  }
}

void NXTabBar::resizeEvent(QResizeEvent *event)
{
  QTabBar::resizeEvent(event);
  d_func()->_updateFixedAreaLayout();
}
