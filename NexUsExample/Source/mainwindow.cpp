#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "NXContentDialog.h"
#include "NXDockWidget.h"
#include "NXEventBus.h"
#include "NXLog.h"
#include "NXMenu.h"
#include "NXMenuBar.h"
#include "NXNavigationRouter.h"
#include "NXProgressBar.h"
#include "NXProgressRing.h"
#include "NXStatusBar.h"
#include "NXSuggestBox.h"
#include "NXText.h"
#include "NXTheme.h"
#include "NXToolBar.h"
#include "NXToolButton.h"
#include "T_About.h"
#include "T_BaseComponents.h"
#include "T_Card.h"
#include "T_Graphics.h"
#include "T_ListView.h"
#include "T_Setting.h"
#include "T_TableView.h"
#include "T_TableWidget.h"
#include "T_TreeView.h"
#ifdef Q_OS_WIN
#include <QTimer>
#include "NXApplication.h"
#endif
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
#include "ExamplePage/T_NXScreen.h"
#endif

#include "ExamplePage/T_Home.h"
#include "ExamplePage/T_Icon.h"
#include "ExamplePage/T_LogWidget.h"
#include "ExamplePage/T_Navigation.h"
#include "ExamplePage/T_Popup.h"
#include "ExamplePage/T_UpdateWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : NXWindow(parent)
    , _rootKey(getNavigationRootKey())
{
  initWindow();

  // 额外布局
  initEdgeLayout();

  // 中心窗口
  initContent();

  // 拦截默认关闭事件
  _closeDialog = new NXContentDialog(this);
  connect(_closeDialog,
          &NXContentDialog::buttonClicked,
          [this]()
  {
    if (NXContentDialog::RightButton) { closeWindow(); }
    else if (NXContentDialog::MiddleButton)
    {
      _closeDialog->close();
      showMinimized();
    }
  });
  this->setIsDefaultClosed(false);
  connect(this, &MainWindow::closeButtonClicked, this, [=]() { _closeDialog->exec(); });

  // 移动到中心
  // moveToCenter();

  //  如果你的windows版本低于Win11 调用原生Mica、Mica-Alt、Acrylic 会导致窗口绘制失效  Dwm_Blur仍可使用
  //    nxTheme->setThemeMode(NXThemeType::Dark);
  //    QTimer::singleShot(1, this, [=]() {
  //        nxApp->setWindowDisplayMode(NXApplicationType::Mica);
  //    });

  //    QTimer::singleShot(1, this, [=]() {
  //        showFullScreen();
  //    });
}

MainWindow::~MainWindow() { delete this->_aboutPage; }

void MainWindow::initWindow()
{
  setFocusPolicy(Qt::StrongFocus);
  // setIsCentralStackedWidgetTransparent(true);
  setWindowIcon(QIcon(":/Resource/Image/Cirno.jpg"));
  resize(1200, 740);
  // nxTheme->setThemeMode(NXThemeType::Dark);
  // setIsNavigationBarEnable(false);
  // setNavigationBarDisplayMode(NXNavigationType::Compact);
  // setWindowButtonFlag(NXAppBarType::MinimizeButtonHint, false);
  setUserInfoCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));
  setUserInfoCardTitle("NX Tool");
  setUserInfoCardSubTitle("Liniyous@gmail.com");
  setWindowTitle("NXWidgetTool");
  // setIsStayTop(true);
  // setUserInfoCardVisible(false);
  // setNavigationBarWidth(260);
  NXText *centralStack = new NXText("这是一个主窗口堆栈页面", this);
  centralStack->setFocusPolicy(Qt::StrongFocus);
  centralStack->setTextPixelSize(32);
  centralStack->setAlignment(Qt::AlignCenter);
  addCentralWidget(centralStack);

  // 窗口绘制模式
  setWindowPixmap(NXThemeType::Light, QPixmap(":/Resource/Image/WindowBase/Miku.png"));
  setWindowPixmap(NXThemeType::Dark, QPixmap(":/Resource/Image/WindowBase/WorldTree.jpg"));
  setWindowMoviePath(NXThemeType::Light, ":/Resource/Image/WindowBase/Miku.gif");
  setWindowMoviePath(NXThemeType::Dark, ":/Resource/Image/WindowBase/WorldTree.gif");
  // setWindowPaintMode(NXWindowType::PaintMode::Pixmap);

  // 自定义AppBar菜单
  NXMenu *appBarMenu = new NXMenu(this);
  appBarMenu->setMenuItemHeight(27);
  connect(appBarMenu->addAction("跳转到一级主要堆栈"), &QAction::triggered, this, [=]() { setCurrentStackIndex(0); });
  connect(appBarMenu->addAction("跳转到二级主要堆栈"), &QAction::triggered, this, [=]() { setCurrentStackIndex(1); });
  connect(appBarMenu->addAction("更改页面切换特效(Scale)"), &QAction::triggered, this, [=]() {
    setStackSwitchMode(NXWindowType::StackSwitchMode::Scale);
  });
  connect(appBarMenu->addNXIconAction(NXIconType::GearComplex, "自定义主窗口设置"), &QAction::triggered, this, [=]() {
    navigation(_settingKey);
  });
  appBarMenu->addSeparator();
  connect(appBarMenu->addNXIconAction(NXIconType::MoonStars, "更改项目主题"), &QAction::triggered, this, [=]() {
    nxTheme->setThemeMode(nxTheme->getThemeMode() == NXThemeType::Light ? NXThemeType::Dark : NXThemeType::Light);
  });
  connect(appBarMenu->addAction("使用原生菜单"), &QAction::triggered, this, [=]() { setCustomMenu(nullptr); });
  setCustomMenu(appBarMenu);

  // 堆栈独立自定义窗口
  QWidget *centralCustomWidget           = new QWidget(this);
  QHBoxLayout *centralCustomWidgetLayout = new QHBoxLayout(centralCustomWidget);
  centralCustomWidgetLayout->setContentsMargins(13, 15, 9, 6);
  NXToolButton *leftButton = new NXToolButton(this);
  leftButton->setNXIcon(NXIconType::AngleLeft);
  leftButton->setEnabled(false);
  connect(
      leftButton, &NXToolButton::clicked, this, [=]() { NXNavigationRouter::getInstance()->navigationRouteBack(); });
  NXToolButton *rightButton = new NXToolButton(this);
  rightButton->setNXIcon(NXIconType::AngleRight);
  rightButton->setEnabled(false);
  connect(rightButton, &NXToolButton::clicked, this, [=]() {
    NXNavigationRouter::getInstance()->navigationRouteForward();
  });
  connect(NXNavigationRouter::getInstance(),
          &NXNavigationRouter::navigationRouterStateChanged,
          this,
          [=](NXNavigationRouterType::RouteMode routeMode)
  {
    switch (routeMode)
    {
      case NXNavigationRouterType::BackValid :
      {
        leftButton->setEnabled(true);
        break;
      }
      case NXNavigationRouterType::BackInvalid :
      {
        leftButton->setEnabled(false);
        break;
      }
      case NXNavigationRouterType::ForwardValid :
      {
        rightButton->setEnabled(true);
        break;
      }
      case NXNavigationRouterType::ForwardInvalid :
      {
        rightButton->setEnabled(false);
        break;
      }
    }
  });
  _windowSuggestBox = new NXSuggestBox(this);
  _windowSuggestBox->setFixedHeight(32);
  _windowSuggestBox->setPlaceholderText("搜索关键字");
  connect(_windowSuggestBox, &NXSuggestBox::suggestionClicked, this, [=](const NXSuggestBox::SuggestData& suggestData) {
    navigation(suggestData.getSuggestData().value("NXPageKey").toString());
  });

  NXText *progressBusyRingText = new NXText("系统运行中", this);
  progressBusyRingText->setIsWrapAnywhere(false);
  progressBusyRingText->setTextPixelSize(15);

  NXProgressRing *progressBusyRing = new NXProgressRing(this);
  progressBusyRing->setBusyingWidth(4);
  progressBusyRing->setFixedSize(28, 28);
  progressBusyRing->setIsBusying(true);

  centralCustomWidgetLayout->addWidget(leftButton);
  centralCustomWidgetLayout->addWidget(rightButton);
  centralCustomWidgetLayout->addWidget(_windowSuggestBox);
  centralCustomWidgetLayout->addStretch();
  centralCustomWidgetLayout->addWidget(progressBusyRingText);
  centralCustomWidgetLayout->addWidget(progressBusyRing);
  setCentralCustomWidget(centralCustomWidget);
}

void MainWindow::initEdgeLayout()
{
  // 菜单栏
  NXMenuBar *menuBar = new NXMenuBar(this);
  menuBar->setFixedHeight(30);
  QWidget *customWidget = new QWidget(this);
  customWidget->setFixedWidth(500);
  QVBoxLayout *customLayout = new QVBoxLayout(customWidget);
  customLayout->setContentsMargins(0, 0, 0, 0);
  customLayout->addWidget(menuBar);
  customLayout->addStretch();
  // this->setMenuBar(menuBar);
  this->setCustomWidget(NXAppBarType::MiddleArea, customWidget);

  menuBar->addNXIconAction(NXIconType::AtomSimple, "动作菜单");
  NXMenu *iconMenu = menuBar->addMenu(NXIconType::Aperture, "图标菜单");
  iconMenu->setMenuItemHeight(27);
  iconMenu->addNXIconAction(NXIconType::BoxCheck, "排序方式", QKeySequence::SelectAll);
  iconMenu->addNXIconAction(NXIconType::Copy, "复制");
  iconMenu->addNXIconAction(NXIconType::MagnifyingGlassPlus, "显示设置");
  iconMenu->addSeparator();
  iconMenu->addNXIconAction(NXIconType::ArrowRotateRight, "刷新");
  iconMenu->addNXIconAction(NXIconType::ArrowRotateLeft, "撤销");
  menuBar->addSeparator();
  NXMenu *shortCutMenu = new NXMenu("快捷菜单(&A)", this);
  shortCutMenu->setMenuItemHeight(27);
  shortCutMenu->addNXIconAction(NXIconType::BoxCheck, "排序方式", QKeySequence::Find);
  shortCutMenu->addNXIconAction(NXIconType::Copy, "复制");
  shortCutMenu->addNXIconAction(NXIconType::MagnifyingGlassPlus, "显示设置");
  shortCutMenu->addSeparator();
  shortCutMenu->addNXIconAction(NXIconType::ArrowRotateRight, "刷新");
  shortCutMenu->addNXIconAction(NXIconType::ArrowRotateLeft, "撤销");
  menuBar->addMenu(shortCutMenu);

  menuBar->addMenu("样例菜单(&B)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");
  menuBar->addMenu("样例菜单(&C)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");
  menuBar->addMenu("样例菜单(&E)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");
  menuBar->addMenu("样例菜单(&F)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");
  menuBar->addMenu("样例菜单(&G)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");

  // 工具栏
  NXToolBar *toolBar = new NXToolBar("工具栏", this);
  toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
  toolBar->setToolBarSpacing(3);
  toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
  toolBar->setIconSize(QSize(25, 25));
  // toolBar->setFloatable(false);
  // toolBar->setMovable(false);
  NXToolButton *toolButton1 = new NXToolButton(this);
  toolButton1->setNXIcon(NXIconType::BadgeCheck);
  toolBar->addWidget(toolButton1);
  NXToolButton *toolButton2 = new NXToolButton(this);
  toolButton2->setNXIcon(NXIconType::ChartUser);
  toolBar->addWidget(toolButton2);
  toolBar->addSeparator();
  NXToolButton *toolButton3 = new NXToolButton(this);
  toolButton3->setNXIcon(NXIconType::Bluetooth);
  toolButton3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  toolButton3->setText("Bluetooth");
  toolBar->addWidget(toolButton3);
  NXToolButton *toolButton4 = new NXToolButton(this);
  toolButton4->setNXIcon(NXIconType::BringFront);
  toolBar->addWidget(toolButton4);
  toolBar->addSeparator();
  NXToolButton *toolButton5 = new NXToolButton(this);
  toolButton5->setNXIcon(NXIconType::ChartSimple);
  toolBar->addWidget(toolButton5);
  NXToolButton *toolButton6 = new NXToolButton(this);
  toolButton6->setNXIcon(NXIconType::FaceClouds);
  toolBar->addWidget(toolButton6);
  NXToolButton *toolButton8 = new NXToolButton(this);
  toolButton8->setNXIcon(NXIconType::Aperture);
  toolBar->addWidget(toolButton8);
  NXToolButton *toolButton9 = new NXToolButton(this);
  toolButton9->setNXIcon(NXIconType::ChartMixed);
  toolBar->addWidget(toolButton9);
  NXToolButton *toolButton10 = new NXToolButton(this);
  toolButton10->setNXIcon(NXIconType::Coins);
  toolBar->addWidget(toolButton10);
  NXToolButton *toolButton11 = new NXToolButton(this);
  toolButton11->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  toolButton11->setNXIcon(NXIconType::AlarmPlus);
  toolButton11->setText("AlarmPlus");
  toolBar->addWidget(toolButton11);
  NXToolButton *toolButton12 = new NXToolButton(this);
  toolButton12->setNXIcon(NXIconType::Crown);
  toolBar->addWidget(toolButton12);
  QAction *test = new QAction(this);
  test->setMenu(new QMenu(this));

  NXProgressBar *progressBar = new NXProgressBar(this);
  progressBar->setMinimum(0);
  progressBar->setMaximum(0);
  progressBar->setFixedWidth(350);
  toolBar->addWidget(progressBar);

  this->addToolBar(Qt::TopToolBarArea, toolBar);

  // 停靠窗口
  NXDockWidget *logDockWidget = new NXDockWidget("日志信息", this);
  logDockWidget->setWidget(new T_LogWidget(this));
  this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
  resizeDocks({ logDockWidget }, { 200 }, Qt::Horizontal);

  NXDockWidget *updateDockWidget = new NXDockWidget("更新内容", this);
  updateDockWidget->setWidget(new T_UpdateWidget(this));
  this->addDockWidget(Qt::RightDockWidgetArea, updateDockWidget);
  resizeDocks({ updateDockWidget }, { 200 }, Qt::Horizontal);

  // 状态栏
  NXStatusBar *statusBar = new NXStatusBar(this);
  NXText *statusText     = new NXText("初始化成功！", this);
  statusText->setTextPixelSize(14);
  statusBar->addWidget(statusText);
  this->setStatusBar(statusBar);
}

void MainWindow::initContent()
{
  _homePage = new T_Home(this);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
  _elaScreenPage = new T_NXScreen(this);
#endif
  _iconPage           = new T_Icon(this);
  _baseComponentsPage = new T_BaseComponents(this);
  _graphicsPage       = new T_Graphics(this);
  _navigationPage     = new T_Navigation(this);
  _popupPage          = new T_Popup(this);
  _cardPage           = new T_Card(this);
  _listViewPage       = new T_ListView(this);
  _tableViewPage      = new T_TableView(this);
  _tableWidgetPage    = new T_TableWidget(this);
  _treeViewPage       = new T_TreeView(this);
  _settingPage        = new T_Setting(this);

  addPageNode("HOME", _homePage, NXIconType::House);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
#ifdef Q_OS_WIN
  // 默认是root 添加一个expander节点
  NXNodeOperateResult dxgiResult = addExpanderNode("NXDxgi", NXIconType::TvMusic);
#else
  addExpanderNode("NXScreenCapture", *dxgiResult, NXIconType::TvMusic);
#endif
  addPageNode("NXScreen", _elaScreenPage, *dxgiResult, 3, NXIconType::ObjectGroup);
#endif
  // navigation(elaScreenWidget->property("NXPageKey").toString());
  addPageNode("NXBaseComponents", _baseComponentsPage, NXIconType::CabinetFiling);

  _viewKey = *addExpanderNode("NXView", _rootKey, NXIconType::CameraViewfinder);
  addPageNode("NXListView", _listViewPage, _viewKey, 9, NXIconType::List);
  addPageNode("NXTableView", _tableViewPage, _viewKey, NXIconType::Table);
  addPageNode("NXTableWidget", _tableWidgetPage, _viewKey, NXIconType::TableCells);
  addPageNode("NXTreeView", _treeViewPage, _viewKey, NXIconType::ListTree);
  expandNavigationNode(_viewKey);

  addPageNode("NXGraphics", _graphicsPage, 9, NXIconType::Paintbrush);
  addPageNode("NXCard", _cardPage, NXIconType::Cards);
  addPageNode("NXNavigation", _navigationPage, NXIconType::LocationArrow);
  addPageNode("NXPopup", _popupPage, NXIconType::Envelope);
  addPageNode("NXIcon", _iconPage, 99, NXIconType::FontCase);

  auto testKey_1 = addExpanderNode("TEST_EXPAND_NODE1", _rootKey, NXIconType::Acorn);
  auto testKey_2 = addExpanderNode("TEST_EXPAND_NODE2", *testKey_1, NXIconType::Acorn);
  addPageNode("TEST_NODE3", new QWidget(this), *testKey_2, NXIconType::Acorn);
  for (int i = 0; i < 10; i++)
  {
    addExpanderNode(QString("TEST_EXPAND_NODE%1").arg(i + 4), *testKey_2, NXIconType::Acorn);
  }
  addExpanderNode("TEST_EXPAND_NODE14", _rootKey, NXIconType::Acorn);
  addExpanderNode("TEST_EXPAND_NODE5", _rootKey, NXIconType::Acorn);
  addExpanderNode("TEST_EXPAND_NODE16", _rootKey, NXIconType::Acorn);

  _aboutKey  = *addFooterNode("About", nullptr, 0, NXIconType::User);
  _aboutPage = new T_About();

  _aboutPage->hide();
  connect(this,
          &NXWindow::navigationNodeClicked,
          this,
          [this](NXNavigationType::NavigationNodeType nodeType, QString nodeKey)
  {
    if (_aboutKey == nodeKey)
    {
      _aboutPage->moveToCenter();
      _aboutPage->show();
    }
  });
  _settingKey = *addFooterNode("Setting", _settingPage, 0, NXIconType::GearComplex);
  connect(this, &MainWindow::userInfoCardClicked, this, [=]() {
    this->navigation(_homePage->property("NXPageKey").toString());
  });
#ifdef Q_OS_WIN
  connect(_homePage, &T_Home::elaScreenNavigation, this, [=]() {
    this->navigation(_elaScreenPage->property("NXPageKey").toString());
  });
#endif
  connect(_homePage, &T_Home::elaBaseComponentNavigation, this, [=]() {
    this->navigation(_baseComponentsPage->property("NXPageKey").toString());
  });
  connect(_homePage, &T_Home::elaSceneNavigation, this, [=]() {
    this->navigation(_graphicsPage->property("NXPageKey").toString());
  });
  connect(_homePage, &T_Home::elaIconNavigation, this, [=]() {
    this->navigation(_iconPage->property("NXPageKey").toString());
  });
  connect(_homePage, &T_Home::elaCardNavigation, this, [=]() {
    this->navigation(_cardPage->property("NXPageKey").toString());
  });
  qDebug() << "已注册的事件列表" << NXEventBus::getInstance()->getRegisteredEventsName();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
  if (getCurrentNavigationIndex() != 2)
  {
    switch (event->button())
    {
      case Qt::BackButton :
      {
        this->setCurrentStackIndex(0);
        break;
      }
      case Qt::ForwardButton :
      {
        this->setCurrentStackIndex(1);
        break;
      }
      default :
      {
        break;
      }
    }
  }
  NXWindow::mouseReleaseEvent(event);
}
