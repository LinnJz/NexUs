#include "NXWindow.h"

#include <QApplication>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QMovie>
#include <QPainter>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScreen>
#include <QStyleOption>
#include <QToolBar>
#include <QtMath>
#include "DeveloperComponents/NXWindowStyle.h"
#include "NXApplication.h"
#include "NXCentralStackedWidget.h"
#include "NXEventBus.h"
#include "NXMenu.h"
#include "NXNavigationBar.h"
#include "NXNavigationRouter.h"
#include "NXTheme.h"
#include "private/NXAppBarPrivate.h"
#include "private/NXNavigationBarPrivate.h"
#include "private/NXWindowPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(NXWindow, int, ThemeChangeTime)
Q_PROPERTY_CREATE_Q_CPP(NXWindow, NXWindowType::StackSwitchMode, StackSwitchMode)
Q_TAKEOVER_NATIVEEVENT_CPP(NXWindow, d_func()->_appBar);

NXWindow::NXWindow(QWidget *parent)
    : QMainWindow { parent }
    , d_ptr(new NXWindowPrivate())
{
  Q_D(NXWindow);
  d->q_ptr             = this;
  d->_pStackSwitchMode = NXWindowType::StackSwitchMode::Popup;
  setProperty("NXBaseClassName", "NXWindow");
  resize(1020, 680); // 默认宽高

  d->_pThemeChangeTime          = 700;
  d->_pNavigationBarDisplayMode = NXNavigationType::NavigationDisplayMode::Auto;

  // 自定义AppBar
  d->_appBar = new NXAppBar(this);
  connect(d->_appBar, &NXAppBar::routeBackButtonClicked, this, []() {
    NXNavigationRouter::getInstance()->navigationRouteBack();
  });
  connect(d->_appBar, &NXAppBar::routeForwardButtonClicked, this, []() {
    NXNavigationRouter::getInstance()->navigationRouteForward();
  });
  connect(d->_appBar, &NXAppBar::closeButtonClicked, this, &NXWindow::closeButtonClicked);
  // 导航栏
  d->_navigationBar = new NXNavigationBar(this);
  // 返回按钮状态变更
  connect(NXNavigationRouter::getInstance(),
          &NXNavigationRouter::navigationRouterStateChanged,
          d,
          &NXWindowPrivate::onNavigationRouterStateChanged);

  // 转发用户卡片点击信号
  connect(d->_navigationBar, &NXNavigationBar::userInfoCardClicked, this, &NXWindow::userInfoCardClicked);
  // 转发点击信号 跳转处理
  connect(d->_navigationBar, &NXNavigationBar::navigationNodeClicked, d, &NXWindowPrivate::onNavigationNodeClicked);
  // 新增窗口
  connect(d->_navigationBar, &NXNavigationBar::navigationNodeAdded, d, &NXWindowPrivate::onNavigationNodeAdded);
  // 移除窗口
  connect(d->_navigationBar, &NXNavigationBar::navigationNodeRemoved, d, &NXWindowPrivate::onNavigationNodeRemoved);
  // 在新窗口打开
  connect(d->_navigationBar, &NXNavigationBar::pageOpenInNewWindow, this, &NXWindow::pageOpenInNewWindow);

  // 导航中心堆栈窗口
  d->_navigationCenterStackedWidget = new NXCentralStackedWidget(this);
  d->_navigationCenterStackedWidget->setContentsMargins(0, 0, 0, 0);
  QWidget *navigationCentralWidget = new QWidget(this);
  navigationCentralWidget->setObjectName("NXWindowNavigationCentralWidget");
  navigationCentralWidget->setStyleSheet("#NXWindowNavigationCentralWidget{background-color:transparent;}");
  navigationCentralWidget->installEventFilter(this);
  d->_centerLayout = new QHBoxLayout(navigationCentralWidget);
  d->_centerLayout->setSpacing(5);
  d->_centerLayout->addWidget(d->_navigationBar);
  d->_centerLayout->addWidget(d->_navigationCenterStackedWidget);
  d->_centerLayout->setContentsMargins(d->_contentsMargins, 0, 0, 0);

  // 事件总线
  d->_focusEvent = new NXEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
  d->_focusEvent->registerAndInit();

  // 展开导航栏
  connect(d->_appBar, &NXAppBar::navigationButtonClicked, d, &NXWindowPrivate::onNavigationButtonClicked);

  // 主题变更动画
  d->_themeMode = nxTheme->getThemeMode();
  connect(nxTheme, &NXTheme::themeModeChanged, d, &NXWindowPrivate::onThemeModeChanged);
  connect(d->_appBar, &NXAppBar::themeChangeButtonClicked, d, &NXWindowPrivate::onThemeReadyChange);
  d->_isInitFinished = true;

  // 中心堆栈窗口
  d->_centerStackedWidget = new NXCentralStackedWidget(this);
  d->_centerStackedWidget->setIsTransparent(true);
  d->_centerStackedWidget->getContainerStackedWidget()->addWidget(navigationCentralWidget);
  setCentralWidget(d->_centerStackedWidget);
  setObjectName("NXWindow");
  setStyleSheet("#NXWindow{background-color:transparent;}");
  setStyle(new NXWindowStyle(style()));

  nxApp->syncWindowDisplayMode(this);
  d->_windowDisplayMode = nxApp->getWindowDisplayMode();
  connect(nxApp, &NXApplication::pWindowDisplayModeChanged, d, &NXWindowPrivate::onWindowDisplayModeChanged);

  d->_pWindowPaintMode = NXWindowType::PaintMode::Normal;
  d->_lightWindowPix   = new QPixmap();
  d->_darkWindowPix    = new QPixmap();

  d->_windowPaintMovie = new QMovie(this);
  connect(d->_windowPaintMovie, &QMovie::frameChanged, this, [=]() { update(); });
}

NXWindow::~NXWindow()
{
  Q_D(NXWindow);
  nxApp->syncWindowDisplayMode(this, false);
  delete this->style();
  delete d->_lightWindowPix;
  delete d->_darkWindowPix;
}

void NXWindow::setIsStayTop(bool isStayTop)
{
  Q_D(NXWindow);
  d->_appBar->setIsStayTop(isStayTop);
  Q_EMIT pIsStayTopChanged();
}

bool NXWindow::getIsStayTop() const { return d_ptr->_appBar->getIsStayTop(); }

void NXWindow::setIsFixedSize(bool isFixedSize)
{
  Q_D(NXWindow);
  d->_appBar->setIsFixedSize(isFixedSize);
  Q_EMIT pIsFixedSizeChanged();
}

bool NXWindow::getIsFixedSize() const { return d_ptr->_appBar->getIsFixedSize(); }

void NXWindow::setIsDefaultClosed(bool isDefaultClosed)
{
  Q_D(NXWindow);
  d->_appBar->setIsDefaultClosed(isDefaultClosed);
  Q_EMIT pIsDefaultClosedChanged();
}

bool NXWindow::getIsDefaultClosed() const { return d_ptr->_appBar->getIsDefaultClosed(); }

void NXWindow::setAppBarHeight(int appBarHeight)
{
  Q_D(NXWindow);
  d->_appBar->setAppBarHeight(appBarHeight);
  Q_EMIT pAppBarHeightChanged();
}

int NXWindow::getAppBarHeight() const
{
  Q_D(const NXWindow);
  return d->_appBar->getAppBarHeight();
}

void NXWindow::setCustomWidget(NXAppBarType::CustomArea customArea,
                               QWidget *widget,
                               QObject *hitTestObject,
                               const QString& hitTestFunctionName)
{
  Q_D(NXWindow);
  d->_appBar->setCustomWidget(customArea, widget, hitTestObject, hitTestFunctionName);
  Q_EMIT customWidgetChanged();
}

QWidget *NXWindow::getCustomWidget(NXAppBarType::CustomArea customArea) const
{
  Q_D(const NXWindow);
  return d->_appBar->getCustomWidget(customArea);
}

void NXWindow::setCentralCustomWidget(QWidget *customWidget)
{
  Q_D(NXWindow);
  d->_navigationCenterStackedWidget->setCustomWidget(customWidget);
  Q_EMIT centralCustomWidgetChanged();
}

QWidget *NXWindow::getCentralCustomWidget() const
{
  Q_D(const NXWindow);
  return d->_navigationCenterStackedWidget->getCustomWidget();
}

void NXWindow::setCustomMenu(QMenu *customMenu)
{
  Q_D(NXWindow);
  d->_appBar->setCustomMenu(customMenu);
  Q_EMIT customMenuChanged();
}

QMenu *NXWindow::getCustomMenu() const
{
  Q_D(const NXWindow);
  return d->_appBar->getCustomMenu();
}

void NXWindow::setIsCentralStackedWidgetTransparent(bool isTransparent)
{
  Q_D(NXWindow);
  d->_navigationCenterStackedWidget->setIsTransparent(isTransparent);
}

bool NXWindow::getIsCentralStackedWidgetTransparent() const
{
  Q_D(const NXWindow);
  return d->_navigationCenterStackedWidget->getIsTransparent();
}

void NXWindow::setIsAllowPageOpenInNewWindow(bool isAllowPageOpenInNewWindow)
{
  Q_D(NXWindow);
  d->_navigationBar->setIsAllowPageOpenInNewWindow(isAllowPageOpenInNewWindow);
  Q_EMIT pIsAllowPageOpenInNewWindowChanged();
}

bool NXWindow::getIsAllowPageOpenInNewWindow() const
{
  Q_D(const NXWindow);
  return d->_navigationBar->getIsAllowPageOpenInNewWindow();
}

void NXWindow::setNavigationBarWidth(int navigationBarWidth)
{
  Q_D(NXWindow);
  d->_navigationBar->setNavigationBarWidth(navigationBarWidth);
  Q_EMIT pNavigationBarWidthChanged();
}

int NXWindow::getNavigationBarWidth() const
{
  Q_D(const NXWindow);
  return d->_navigationBar->getNavigationBarWidth();
}

void NXWindow::setCurrentStackIndex(int currentStackIndex)
{
  Q_D(NXWindow);
  if (currentStackIndex >= d->_centerStackedWidget->getContainerStackedWidget()->count() || currentStackIndex < 0 ||
      currentStackIndex == d->_centralStackTargetIndex)
  {
    return;
  }
  d->_centralStackTargetIndex = currentStackIndex;
  QVariantMap routeData;
  int currentCenterStackedWidgetIndex = d->_centerStackedWidget->getContainerStackedWidget()->currentIndex();
  routeData.insert("NXBackCentralStackIndex", currentCenterStackedWidgetIndex);
  routeData.insert("NXForwardCentralStackIndex", currentStackIndex);
  NXNavigationRouter::getInstance()->navigationRoute(d, "onNavigationRoute", routeData);
  d->_centerStackedWidget->doWindowStackSwitch(d->_pStackSwitchMode, currentStackIndex, false);
  Q_EMIT pCurrentStackIndexChanged();
}

int NXWindow::getCurrentStackIndex() const
{
  Q_D(const NXWindow);
  return d->_centerStackedWidget->getContainerStackedWidget()->currentIndex();
}

void NXWindow::setNavigationBarDisplayMode(NXNavigationType::NavigationDisplayMode navigationBarDisplayMode)
{
  Q_D(NXWindow);
  d->_pNavigationBarDisplayMode       = navigationBarDisplayMode;
  d->_currentNavigationBarDisplayMode = d->_pNavigationBarDisplayMode;
  bool isVisible                      = this->isVisible();
  switch (d->_pNavigationBarDisplayMode)
  {
    case NXNavigationType::Auto :
    {
      d->_doNavigationDisplayModeChange();
      break;
    }
    case NXNavigationType::Minimal :
    {
      d->_navigationBar->setDisplayMode(NXNavigationType::Minimal, isVisible);
      break;
    }
    case NXNavigationType::Compact :
    {
      d->_navigationBar->setDisplayMode(NXNavigationType::Compact, isVisible);
      break;
    }
    case NXNavigationType::Maximal :
    {
      d->_navigationBar->setDisplayMode(NXNavigationType::Maximal, isVisible);
      break;
    }
  }
  Q_EMIT pNavigationBarDisplayModeChanged();
}

NXNavigationType::NavigationDisplayMode NXWindow::getNavigationBarDisplayMode() const
{
  Q_D(const NXWindow);
  return d->_pNavigationBarDisplayMode;
}

void NXWindow::setWindowPaintMode(NXWindowType::PaintMode windowPaintMode)
{
  Q_D(NXWindow);
  if (d->_windowPaintMovie->state() == QMovie::Running) { d->_windowPaintMovie->stop(); }
  if (windowPaintMode == NXWindowType::PaintMode::Movie)
  {
    d->_windowPaintMovie->setFileName(d->_themeMode == NXThemeType::Light ? d->_lightWindowMoviePath
                                                                          : d->_darkWindowMoviePath);
    d->_windowPaintMovie->start();
  }
  d->_pWindowPaintMode = windowPaintMode;
  update();
  Q_EMIT pWindowPaintModeChanged();
}

NXWindowType::PaintMode NXWindow::getWindowPaintMode() const
{
  Q_D(const NXWindow);
  return d->_pWindowPaintMode;
}

void NXWindow::moveToCenter()
{
  if (isMaximized() || isFullScreen()) { return; }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
  auto geometry = screen()->availableGeometry();
#else
  auto geometry = qApp->screenAt(this->geometry().center())->geometry();
#endif
  setGeometry((geometry.left() + geometry.right() - width()) / 2,
              (geometry.top() + geometry.bottom() - height()) / 2,
              width(),
              height());
}

void NXWindow::setIsNavigationBarEnable(bool isVisible)
{
  Q_D(NXWindow);
  d->_isNavigationEnable = isVisible;
  d->_navigationBar->setVisible(isVisible);
  d->_centerLayout->setContentsMargins(isVisible ? d->_contentsMargins : 0, 0, 0, 0);
  d->_navigationCenterStackedWidget->setIsHasRadius(isVisible);
}

bool NXWindow::getIsNavigationBarEnable() const { return d_ptr->_isNavigationEnable; }

void NXWindow::setIsLeftButtonPressedToggleNavigation(bool isPressed)
{
  Q_D(NXWindow);
  d->_navigationBar->setIsLeftButtonPressedToggleNavigation(isPressed);
}

void NXWindow::setNavigationNodeDragAndDropEnable(bool isEnable)
{
  Q_D(NXWindow);
  d->_navigationBar->setNavigationNodeDragAndDropEnable(isEnable);
}

void NXWindow::setUserInfoCardVisible(bool isVisible)
{
  Q_D(NXWindow);
  d->_navigationBar->setUserInfoCardVisible(isVisible);
}

void NXWindow::setUserInfoCardPixmap(QPixmap pix)
{
  Q_D(NXWindow);
  d->_navigationBar->setUserInfoCardPixmap(pix);
}

void NXWindow::setUserInfoCardTitle(const QString& title)
{
  Q_D(NXWindow);
  d->_navigationBar->setUserInfoCardTitle(title);
}

void NXWindow::setUserInfoCardSubTitle(const QString& subTitle)
{
  Q_D(NXWindow);
  d->_navigationBar->setUserInfoCardSubTitle(subTitle);
}

NXNavigationType::NodeOperateError NXWindow::setNavigationNodeTitle(const QString& nodeTitle, const QString& nodeKey)
{
  Q_D(NXWindow);
  return d->_navigationBar->setNavigationNodeTitle(nodeTitle, nodeKey);
}

QString NXWindow::getNavigationNodeTitle(const QString& nodeKey) const
{
  Q_D(const NXWindow);
  return d->_navigationBar->getNavigationNodeTitle(nodeKey);
}

NXNavigationType::NodeOperateError NXWindow::navigationPageNodeSwitch(const QString& targetPageKey)
{
  Q_D(NXWindow);
  return d->_navigationBar->navigationPageNodeSwitch(targetPageKey);
}

void NXWindow::setNavigationPageOpenPolicy(std::function<void(const QString& /*nodeKey*/)>&& openNavigationPageFunc)
{
  Q_D(NXWindow);
  d->_navigationBar->setNavigationPageOpenPolicy(std::move(openNavigationPageFunc));
}

QString NXWindow::addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome) const
{
  Q_D(const NXWindow);
  return d->_navigationBar->addExpanderNode(expanderTitle, awesome);
}

NXNodeOperateResult NXWindow::addExpanderNode(const QString& expanderTitle,
                                              const QString& targetExpanderKey,
                                              NXIconType::IconName awesome) const
{
  Q_D(const NXWindow);
  return d->_navigationBar->addExpanderNode(expanderTitle, targetExpanderKey, awesome);
}

NXNodeOperateResult NXWindow::addPageNode(const QString& pageTitle, QWidget *page, NXIconType::IconName awesome)
{
  Q_D(NXWindow);
  NXNodeOperateResult returnData = d->_navigationBar->addPageNode(pageTitle, page, awesome);
  if (returnData.has_value()) { d->_pageMetaMap.insert(page->property("NXPageKey").toString(), page->metaObject()); }
  return returnData;
}

NXNodeOperateResult NXWindow::addPageNode(const QString& pageTitle,
                                          QWidget *page,
                                          const QString& targetExpanderKey,
                                          NXIconType::IconName awesome)
{
  Q_D(NXWindow);
  NXNodeOperateResult returnData = d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, awesome);
  if (returnData.has_value()) { d->_pageMetaMap.insert(page->property("NXPageKey").toString(), page->metaObject()); }
  return returnData;
}

NXNodeOperateResult
NXWindow::addPageNode(const QString& pageTitle, QWidget *page, int keyPoints, NXIconType::IconName awesome)
{
  Q_D(NXWindow);
  NXNodeOperateResult returnData = d->_navigationBar->addPageNode(pageTitle, page, keyPoints, awesome);
  if (returnData.has_value()) { d->_pageMetaMap.insert(page->property("NXPageKey").toString(), page->metaObject()); }
  return returnData;
}

NXNodeOperateResult NXWindow::addPageNode(const QString& pageTitle,
                                          QWidget *page,
                                          const QString& targetExpanderKey,
                                          int keyPoints,
                                          NXIconType::IconName awesome)
{
  Q_D(NXWindow);
  NXNodeOperateResult returnData =
      d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, awesome);
  if (returnData.has_value()) { d->_pageMetaMap.insert(page->property("NXPageKey").toString(), page->metaObject()); }
  return returnData;
}

QString NXWindow::addCategoryNode(const QString& categoryTitle)
{
  Q_D(const NXWindow);
  return d->_navigationBar->addCategoryNode(categoryTitle);
}

NXNodeOperateResult NXWindow::addCategoryNode(const QString& categoryTitle, const QString& targetExpanderKey)
{
  Q_D(const NXWindow);
  return d->_navigationBar->addCategoryNode(categoryTitle, targetExpanderKey);
}

NXNodeOperateResult
NXWindow::addFooterNode(const QString& footerTitle, int keyPoints, NXIconType::IconName awesome) const
{
  Q_D(const NXWindow);
  return d->_navigationBar->addFooterNode(footerTitle, nullptr, keyPoints, awesome);
}

NXNodeOperateResult
NXWindow::addFooterNode(const QString& footerTitle, QWidget *page, int keyPoints, NXIconType::IconName awesome)
{
  Q_D(NXWindow);
  NXNodeOperateResult returnData = d->_navigationBar->addFooterNode(footerTitle, page, keyPoints, awesome);
  if (page && returnData.has_value())
  {
    d->_pageMetaMap.insert(page->property("NXPageKey").toString(), page->metaObject());
  }
  return returnData;
}

void NXWindow::addCentralWidget(QWidget *centralWidget)
{
  Q_D(NXWindow);
  if (!centralWidget) { return; }
  d->_centerStackedWidget->getContainerStackedWidget()->addWidget(centralWidget);
}

QWidget *NXWindow::getCentralWidget(int index) const
{
  Q_D(const NXWindow);
  if (index >= d->_centerStackedWidget->getContainerStackedWidget()->count() || index < 1) { return nullptr; }
  return d->_centerStackedWidget->getContainerStackedWidget()->widget(index);
}

QString NXWindow::getNavigationRootKey() const
{
  Q_D(const NXWindow);
  return d->_navigationBar->getNavigationRootKey();
}

bool NXWindow::getNavigationNodeIsExpanded(const QString& expanderKey) const
{
  Q_D(const NXWindow);
  return d->_navigationBar->getNavigationNodeIsExpanded(expanderKey);
}

void NXWindow::expandNavigationNode(const QString& expanderKey)
{
  Q_D(NXWindow);
  d->_navigationBar->expandNavigationNode(expanderKey);
}

void NXWindow::collapseNavigationNode(const QString& expanderKey)
{
  Q_D(NXWindow);
  d->_navigationBar->collapseNavigationNode(expanderKey);
}

void NXWindow::removeNavigationNode(const QString& nodeKey) const
{
  Q_D(const NXWindow);
  d->_navigationBar->removeNavigationNode(nodeKey);
}

int NXWindow::getPageOpenInNewWindowCount(const QString& nodeKey) const
{
  Q_D(const NXWindow);
  return d->_navigationBar->getPageOpenInNewWindowCount(nodeKey);
}

void NXWindow::backtrackNavigationNode(const QString& nodeKey)
{
  Q_D(NXWindow);
  const QMetaObject *meta = d->_pageMetaMap.value(nodeKey);
  if (!meta) { return; }
  QWidget *widget = dynamic_cast<QWidget *>(meta->newInstance());
  if (widget)
  {
    auto originWidget = d->_routeMap[nodeKey];
    int currentIndex  = d->_navigationCenterStackedWidget->getContainerStackedWidget()->currentIndex();
    int originIndex   = d->_navigationCenterStackedWidget->getContainerStackedWidget()->indexOf(originWidget);
    widget->setProperty("NXPageKey", nodeKey);
    d->_routeMap[nodeKey] = widget;
    d->_navigationCenterStackedWidget->getContainerStackedWidget()->insertWidget(originIndex, widget);
    d->_navigationCenterStackedWidget->getContainerStackedWidget()->setCurrentIndex(currentIndex);
    d->_navigationCenterStackedWidget->getContainerStackedWidget()->removeWidget(originWidget);
    originWidget->deleteLater();
  }
}

void NXWindow::setNodeKeyPoints(const QString& nodeKey, int keyPoints)
{
  Q_D(NXWindow);
  d->_navigationBar->setNodeKeyPoints(nodeKey, keyPoints);
}

int NXWindow::getNodeKeyPoints(const QString& nodeKey) const
{
  Q_D(const NXWindow);
  return d->_navigationBar->getNodeKeyPoints(nodeKey);
}

void NXWindow::navigation(const QString& pageKey)
{
  Q_D(NXWindow);
  d->_navigationBar->navigation(pageKey);
}

int NXWindow::getCurrentNavigationIndex() const
{
  Q_D(const NXWindow);
  return d->_navigationCenterStackedWidget->getContainerStackedWidget()->currentIndex();
}

QString NXWindow::getCurrentNavigationPageKey() const
{
  Q_D(const NXWindow);
  return d->_navigationCenterStackedWidget->getContainerStackedWidget()
      ->currentWidget()
      ->property("NXPageKey")
      .toString();
}

QList<NXSuggestBox::SuggestData> NXWindow::getNavigationSuggestDataList() const
{
  Q_D(const NXWindow);
  return d->_navigationBar->getSuggestDataList();
}

void NXWindow::setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable)
{
  Q_D(NXWindow);
  d->_appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

void NXWindow::setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags)
{
  Q_D(NXWindow);
  d->_appBar->setWindowButtonFlags(buttonFlags);
}

NXAppBarType::ButtonFlags NXWindow::getWindowButtonFlags() const { return d_ptr->_appBar->getWindowButtonFlags(); }

void NXWindow::setWindowMoviePath(NXThemeType::ThemeMode themeMode, const QString& moviePath)
{
  Q_D(NXWindow);
  if (themeMode == NXThemeType::ThemeMode::Light) { d->_lightWindowMoviePath = moviePath; }
  else
  {
    d->_darkWindowMoviePath = moviePath;
  }
  if (d->_themeMode == themeMode && d->_pWindowPaintMode == NXWindowType::PaintMode::Movie)
  {
    if (d->_windowPaintMovie->state() == QMovie::Running) { d->_windowPaintMovie->stop(); }
    d->_windowPaintMovie->setFileName(moviePath);
    d->_windowPaintMovie->start();
  }
}

QString NXWindow::getWindowMoviePath(NXThemeType::ThemeMode themeMode) const
{
  Q_D(const NXWindow);
  return themeMode == NXThemeType::Light ? d->_lightWindowMoviePath : d->_darkWindowMoviePath;
}

void NXWindow::setWindowMovieRate(qreal rate)
{
  Q_D(NXWindow);
  d->_windowPaintMovie->setSpeed(rate * 100);
}

qreal NXWindow::getWindowMovieRate() const
{
  Q_D(const NXWindow);
  return d->_windowPaintMovie->speed() / 100.0;
}

void NXWindow::setWindowPixmap(NXThemeType::ThemeMode themeMode, const QPixmap& pixmap)
{
  Q_D(NXWindow);
  if (themeMode == NXThemeType::ThemeMode::Light) { *d->_lightWindowPix = pixmap; }
  else
  {
    *d->_darkWindowPix = pixmap;
  }
  update();
}

QPixmap NXWindow::getWindowPixmap(NXThemeType::ThemeMode themeMode) const
{
  Q_D(const NXWindow);
  return themeMode == NXThemeType::Light ? *d->_lightWindowPix : *d->_darkWindowPix;
}

void NXWindow::closeWindow()
{
  Q_D(NXWindow);
  d->_isWindowClosing = true;
  d->_appBar->closeWindow();
}

bool NXWindow::eventFilter(QObject *watched, QEvent *event)
{
  Q_D(NXWindow);
  switch (event->type())
  {
    case QEvent::Resize :
    {
      d->_doNavigationDisplayModeChange();
      break;
    }
    default :
    {
      break;
    }
  }
  return QMainWindow::eventFilter(watched, event);
}

QMenu *NXWindow::createPopupMenu()
{
  NXMenu *menu                     = nullptr;
  QList<QDockWidget *> dockwidgets = findChildren<QDockWidget *>();
  if (dockwidgets.size())
  {
    menu = new NXMenu(this);
    for (int i = 0; i < dockwidgets.size(); ++i)
    {
      QDockWidget *dockWidget = dockwidgets.at(i);
      if (dockWidget->parentWidget() == this) { menu->addAction(dockwidgets.at(i)->toggleViewAction()); }
    }
    menu->addSeparator();
  }

  QList<QToolBar *> toolbars = findChildren<QToolBar *>();
  if (toolbars.size())
  {
    if (!menu) { menu = new NXMenu(this); }
    for (int i = 0; i < toolbars.size(); ++i)
    {
      QToolBar *toolBar = toolbars.at(i);
      if (toolBar->parentWidget() == this) { menu->addAction(toolbars.at(i)->toggleViewAction()); }
    }
  }
  if (menu) { menu->setMenuItemHeight(28); }
  return menu;
}

void NXWindow::paintEvent(QPaintEvent *event)
{
  Q_D(NXWindow);
  QPainter painter(this);
  painter.save();
  painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
  switch (d->_windowDisplayMode)
  {
    case NXApplicationType::Normal :
    {
      switch (d->_pWindowPaintMode)
      {
        case NXWindowType::Normal :
        {
          painter.setPen(Qt::NoPen);
          painter.setBrush(NXThemeColor(d->_themeMode, WindowBase));
          painter.drawRect(rect());
          break;
        }
        case NXWindowType::Pixmap :
        {
          QPixmap *pix            = d->_themeMode == NXThemeType::Light ? d->_lightWindowPix : d->_darkWindowPix;
          qreal windowAspectRatio = (qreal) rect().width() / rect().height();
          qreal pixAspectRatio    = (qreal) pix->width() / pix->height();
          int targetPixWidth, targetPixHeight;
          if (windowAspectRatio < pixAspectRatio)
          {
            targetPixWidth  = qRound(pix->width() * windowAspectRatio / pixAspectRatio);
            targetPixHeight = pix->height();
          }
          else
          {
            targetPixWidth  = pix->width();
            targetPixHeight = qRound(pix->height() * pixAspectRatio / windowAspectRatio);
          }
          painter.drawPixmap(rect(),
                             *pix,
                             QRect((pix->width() - targetPixWidth) / 2,
                                   (pix->height() - targetPixHeight) / 2,
                                   targetPixWidth,
                                   targetPixHeight));
          break;
        }
        case NXWindowType::Movie :
        {
          QPixmap pix             = d->_windowPaintMovie->currentPixmap();
          qreal windowAspectRatio = (qreal) rect().width() / rect().height();
          qreal pixAspectRatio    = (qreal) pix.width() / pix.height();
          int targetPixWidth, targetPixHeight;
          if (windowAspectRatio < pixAspectRatio)
          {
            targetPixWidth  = qRound(pix.width() * windowAspectRatio / pixAspectRatio);
            targetPixHeight = pix.height();
          }
          else
          {
            targetPixWidth  = pix.width();
            targetPixHeight = qRound(pix.height() * pixAspectRatio / windowAspectRatio);
          }
          painter.drawPixmap(rect(),
                             pix,
                             QRect((pix.width() - targetPixWidth) / 2,
                                   (pix.height() - targetPixHeight) / 2,
                                   targetPixWidth,
                                   targetPixHeight));
          break;
        }
        default :
        {
          break;
        }
      }
    }
    default :
    {
      break;
    }
  }
  painter.restore();
}
