#include "NXScrollPage.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "NXBreadcrumbBar.h"
#include "NXNavigationRouter.h"
#include "NXScrollArea.h"
#include "NXScrollBar.h"
#include "private/NXScrollPagePrivate.h"

NXScrollPage::NXScrollPage(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new NXScrollPagePrivate())
{
  Q_D(NXScrollPage);
  setProperty("NXBaseClassName", "NXScrollPage");
  d->q_ptr          = this;
  d->_pCustomWidget = nullptr;
  d->_breadcrumbBar = new NXBreadcrumbBar(this);
  d->_breadcrumbBar->setTextPixelSize(28);
  connect(d->_breadcrumbBar,
          &NXBreadcrumbBar::breadcrumbClicked,
          this,
          [=](QString breadcrumb, QStringList lastBreadcrumbList)
  {
    if (d->_centralWidgetMap.contains(breadcrumb))
    {
      int widgetIndex = d->_centralWidgetMap.value(breadcrumb);
      d->_switchCentralStackIndex(widgetIndex, d->_navigationTargetIndex);
      d->_navigationTargetIndex = widgetIndex;
      QVariantMap routeData     = QVariantMap();
      routeData.insert("NXScrollPageCheckSumKey", "BreadcrumbClicked");
      routeData.insert("NXBackBreadcrumbList", lastBreadcrumbList);
      routeData.insert("NXForwardBreadcrumbList", d->_breadcrumbBar->getBreadcrumbList());
      NXNavigationRouter::getInstance()->navigationRoute(d, "onNavigationRoute", routeData);
    }
  });
  d->_pageTitleLayout = new QHBoxLayout();
  d->_pageTitleLayout->setContentsMargins(0, 0, 0, 0);
  d->_pageTitleLayout->addWidget(d->_breadcrumbBar);

  d->_centralStackedWidget = new QStackedWidget(this);
  d->_centralStackedWidget->setContentsMargins(0, 0, 0, 0);

  d->_mainLayout = new QVBoxLayout(this);
  d->_mainLayout->setSpacing(0);
  d->_mainLayout->setContentsMargins(0, 0, 0, 0);
  d->_mainLayout->addLayout(d->_pageTitleLayout);
  d->_mainLayout->addWidget(d->_centralStackedWidget);
  setContentsMargins(20, 20, 0, 0);
}

NXScrollPage::~NXScrollPage() { }

void NXScrollPage::addCentralWidget(QWidget *centralWidget,
                                    bool isWidgetResizeable,
                                    bool isVerticalGrabGesture,
                                    qreal mousePressEventDelay,
                                    Qt::ScrollBarPolicy vScrollBarPolicy,
                                    Qt::ScrollBarPolicy hScrollBarPolicy)
{
  Q_D(NXScrollPage);
  if (!centralWidget) { return; }
  if (centralWidget->windowTitle().isEmpty())
  {
    centralWidget->setWindowTitle(QString("Page_%1").arg(d->_centralStackedWidget->count()));
  }
  if (d->_centralStackedWidget->count() == 0) { d->_breadcrumbBar->appendBreadcrumb(centralWidget->windowTitle()); }
  NXScrollArea *scrollArea = new NXScrollArea(this);
  scrollArea->setMouseTracking(true);
  scrollArea->setIsAnimation(Qt::Vertical, true);
  scrollArea->setWidgetResizable(isWidgetResizeable);
  scrollArea->setIsGrabGesture(isVerticalGrabGesture, mousePressEventDelay);
  scrollArea->setIsOverShoot(Qt::Vertical, true);

  scrollArea->setVerticalScrollBarPolicy(vScrollBarPolicy);
  scrollArea->setHorizontalScrollBarPolicy(hScrollBarPolicy);
  NXScrollBar *floatVScrollBar = new NXScrollBar(scrollArea->verticalScrollBar(), scrollArea);
  floatVScrollBar->setIsAnimation(true);

  QWidget *scrollPageContainer = new QWidget(this);
  scrollPageContainer->setObjectName("NXScrollPageContainer");
  scrollPageContainer->setStyleSheet("#NXScrollPageContainer{background-color:transparent;}");
  QVBoxLayout *scrollPageContainerLayout = new QVBoxLayout(scrollPageContainer);
  scrollPageContainerLayout->setContentsMargins(0, 0, 0, 0);
  scrollPageContainerLayout->addWidget(centralWidget);
  scrollArea->setWidget(scrollPageContainer);

  d->_centralWidgetMap.insert(centralWidget->windowTitle(), d->_centralStackedWidget->count());
  d->_centralStackedWidget->addWidget(scrollArea);
}

void NXScrollPage::setCustomWidget(QWidget *widget)
{
  Q_D(NXScrollPage);
  if (!widget || widget == this) { return; }
  if (d->_pCustomWidget) { d->_mainLayout->removeWidget(d->_pCustomWidget); }
  d->_mainLayout->insertWidget(1, widget);
  d->_pCustomWidget = widget;
  Q_EMIT pCustomWidgetChanged();
}

QWidget *NXScrollPage::getCustomWidget() const
{
  Q_D(const NXScrollPage);
  return d->_pCustomWidget;
}

void NXScrollPage::navigation(int widgetIndex, bool isLogRoute)
{
  Q_D(NXScrollPage);
  if (widgetIndex >= d->_centralStackedWidget->count() || d->_navigationTargetIndex == widgetIndex) { return; }
  d->_switchCentralStackIndex(widgetIndex, d->_navigationTargetIndex);
  d->_navigationTargetIndex = widgetIndex;
  QString pagetTitle        = d->_centralWidgetMap.key(widgetIndex);
  if (isLogRoute)
  {
    QVariantMap routeData = QVariantMap();
    routeData.insert("NXScrollPageCheckSumKey", "Navigation");
    QStringList breadcrumbList = d->_breadcrumbBar->getBreadcrumbList();
    routeData.insert("NXBackPageTitle", breadcrumbList.last());
    routeData.insert("NXForwardPageTitle", pagetTitle);
    NXNavigationRouter::getInstance()->navigationRoute(d, "onNavigationRoute", routeData);
  }
  d->_breadcrumbBar->appendBreadcrumb(pagetTitle);
}

void NXScrollPage::setPageTitleSpacing(int spacing)
{
  Q_D(NXScrollPage);
  d->_pageTitleLayout->takeAt(0);
  d->_pageTitleLayout->addSpacing(spacing);
  d->_pageTitleLayout->addWidget(d->_breadcrumbBar);
}

int NXScrollPage::getPageTitleSpacing() const { return d_ptr->_pageTitleSpacing; }

void NXScrollPage::setTitleVisible(bool isVisible)
{
  Q_D(NXScrollPage);
  d->_breadcrumbBar->setVisible(isVisible);
}
