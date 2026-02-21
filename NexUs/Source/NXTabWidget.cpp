#include "NXTabWidget.h"

#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include "NXTabBar.h"
#include "NXTheme.h"
#include "private/NXTabWidgetPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(NXTabWidget, bool, IsTabTransparent);
Q_PROPERTY_CREATE_Q_CPP(NXTabWidget, bool, IsContainerAcceptDrops);

NXTabWidget::NXTabWidget(QWidget *parent)
    : QTabWidget(parent)
    , d_ptr(new NXTabWidgetPrivate())
{
  Q_D(NXTabWidget);
  d->q_ptr                    = this;
  d->_pIsContainerAcceptDrops = false;
  d->_pIsTabTransparent           = false;
  d->_pIsSelectedIndicatorVisible = true;
  d->_pTabCornerRadius            = 7;
  d->_pTabBarStyle            = NXTabBarType::Google;
  setObjectName("NXTabWidget");
  setAcceptDrops(true);
  d->_tabBar = new NXTabBar(this);
  setTabBar(d->_tabBar);
  d->_tabBar->setTabBarStyle(d->_pTabBarStyle);
  d->_tabBar->setTabCornerRadius(d->_pTabCornerRadius);
  d->_tabBar->setIsSelectedIndicatorVisible(d->_pIsSelectedIndicatorVisible);
  connect(d->_tabBar, &NXTabBar::tabDragCreate, d, &NXTabWidgetPrivate::onTabDragCreate);
  connect(d->_tabBar, &NXTabBar::tabDragEnter, d, &NXTabWidgetPrivate::onTabDragEnter);
  connect(d->_tabBar, &NXTabBar::tabDragLeave, d, &NXTabWidgetPrivate::onTabDragLeave);
  connect(d->_tabBar, &NXTabBar::tabDragDrop, d, &NXTabWidgetPrivate::onTabDragDrop);
  connect(d->_tabBar, &NXTabBar::tabCloseRequested, d, &NXTabWidgetPrivate::onTabCloseRequested);
}

NXTabWidget::~NXTabWidget()
{
  Q_D(NXTabWidget);
  d->_clearAllTabWidgetList();
}

void NXTabWidget::setTabSize(QSize tabSize)
{
  Q_D(NXTabWidget);
  d->_tabBar->setTabSize(tabSize);
}

QSize NXTabWidget::getTabSize() const
{
  Q_D(const NXTabWidget);
  return d->_tabBar->getTabSize();
}

void NXTabWidget::setTabBarStyle(NXTabBarType::TabBarStyle style)
{
  Q_D(NXTabWidget);
  d->_pTabBarStyle = style;
  if (d->_tabBar) { d->_tabBar->setTabBarStyle(style); }
  if (d->_customTabBar) { d->_customTabBar->setTabBarStyle(style); }
}

NXTabBarType::TabBarStyle NXTabWidget::getTabBarStyle() const
{
  Q_D(const NXTabWidget);
  return d->_pTabBarStyle;
}

void NXTabWidget::setTabCornerRadius(int radius)
{
  Q_D(NXTabWidget);
  d->_pTabCornerRadius = radius;
  if (d->_tabBar) { d->_tabBar->setTabCornerRadius(radius); }
  if (d->_customTabBar) { d->_customTabBar->setTabCornerRadius(radius); }
}

int NXTabWidget::getTabCornerRadius() const
{
  Q_D(const NXTabWidget);
  return d->_pTabCornerRadius;
}

void NXTabWidget::setIsSelectedIndicatorVisible(bool isVisible)
{
  Q_D(NXTabWidget);
  d->_pIsSelectedIndicatorVisible = isVisible;
  if (d->_tabBar) { d->_tabBar->setIsSelectedIndicatorVisible(isVisible); }
  if (d->_customTabBar) { d->_customTabBar->setIsSelectedIndicatorVisible(isVisible); }
}

bool NXTabWidget::getIsSelectedIndicatorVisible() const
{
  Q_D(const NXTabWidget);
  return d->_pIsSelectedIndicatorVisible;
}

void NXTabWidget::setTabBarThemeColor(NXThemeType::ThemeMode themeMode,
                                     NXTabBarType::TabBarColorRole role,
                                     const QColor& color)
{
  Q_D(NXTabWidget);
  NXThemeType::ThemeColor themeColor = NXThemeType::TabBarBase;
  switch (role)
  {
    case NXTabBarType::Base : themeColor = NXThemeType::TabBarBase; break;
    case NXTabBarType::Selected : themeColor = NXThemeType::TabBarSelected; break;
    case NXTabBarType::Hover : themeColor = NXThemeType::TabBarHover; break;
    default : break;
  }
  nxTheme->setThemeColor(themeMode, themeColor, color);
  if (d->_tabBar) { d->_tabBar->update(); }
  if (d->_customTabBar) { d->_customTabBar->update(); }
}

QColor NXTabWidget::getTabBarThemeColor(NXThemeType::ThemeMode themeMode, NXTabBarType::TabBarColorRole role) const
{
  NXThemeType::ThemeColor themeColor = NXThemeType::TabBarBase;
  switch (role)
  {
    case NXTabBarType::Base : themeColor = NXThemeType::TabBarBase; break;
    case NXTabBarType::Selected : themeColor = NXThemeType::TabBarSelected; break;
    case NXTabBarType::Hover : themeColor = NXThemeType::TabBarHover; break;
    default : break;
  }
  return nxTheme->getThemeColor(themeMode, themeColor);
}

void NXTabWidget::addLeftFixedButton(QWidget *widget)
{
  Q_D(NXTabWidget);
  if (d->_tabBar) { d->_tabBar->addLeftFixedButton(widget); }
  if (d->_customTabBar) { d->_customTabBar->addLeftFixedButton(widget); }
}

void NXTabWidget::addRightFixedButton(QWidget *widget)
{
  Q_D(NXTabWidget);
  if (d->_tabBar) { d->_tabBar->addRightFixedButton(widget); }
  if (d->_customTabBar) { d->_customTabBar->addRightFixedButton(widget); }
}

void NXTabWidget::removeLeftFixedButton(QWidget *widget)
{
  Q_D(NXTabWidget);
  if (d->_tabBar) { d->_tabBar->removeLeftFixedButton(widget); }
  if (d->_customTabBar) { d->_customTabBar->removeLeftFixedButton(widget); }
}

void NXTabWidget::removeRightFixedButton(QWidget *widget)
{
  Q_D(NXTabWidget);
  if (d->_tabBar) { d->_tabBar->removeRightFixedButton(widget); }
  if (d->_customTabBar) { d->_customTabBar->removeRightFixedButton(widget); }
}

void NXTabWidget::clearLeftFixedButtons()
{
  Q_D(NXTabWidget);
  if (d->_tabBar) { d->_tabBar->clearLeftFixedButtons(); }
  if (d->_customTabBar) { d->_customTabBar->clearLeftFixedButtons(); }
}

void NXTabWidget::clearRightFixedButtons()
{
  Q_D(NXTabWidget);
  if (d->_tabBar) { d->_tabBar->clearRightFixedButtons(); }
  if (d->_customTabBar) { d->_customTabBar->clearRightFixedButtons(); }
}

void NXTabWidget::setTabPosition(TabPosition position)
{
  if (position == QTabWidget::North || position == QTabWidget::South) { QTabWidget::setTabPosition(position); }
}

void NXTabWidget::paintEvent(QPaintEvent *event)
{
  Q_D(NXTabWidget);
  if (!d->_pIsTabTransparent) { QTabWidget::paintEvent(event); }
}

void NXTabWidget::dragEnterEvent(QDragEnterEvent *event)
{
  if (event->mimeData()->property("DragType").toString() == "NXTabBarDrag") { event->acceptProposedAction(); }
  QTabWidget::dragEnterEvent(event);
}

void NXTabWidget::dropEvent(QDropEvent *event)
{
  Q_D(NXTabWidget);
  if (d->_pIsContainerAcceptDrops && event->mimeData()->property("NXTabWidgetObject").value<NXTabWidget *>() != this)
  {
    QMimeData *data = const_cast<QMimeData *>(event->mimeData());
    data->setProperty("TabDropIndex", count());
    d->onTabDragDrop(data);
    event->accept();
  }
  QTabWidget::dropEvent(event);
}

void NXTabWidget::tabInserted(int index)
{
  Q_D(NXTabWidget);
  QWidget *tabWidget = widget(index);
  if (!tabWidget->property("IsMetaWidget").toBool() && !tabWidget->property("NXOriginTabWidget").isValid())
  {
    d->_allTabWidgetList.append(widget(index));
  }
  QTabWidget::tabInserted(index);
}
