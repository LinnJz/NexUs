# NexUs API 文档

> 本文档由 `scripts/generate_docs.py` 自动生成，请勿手动编辑。
>
> 共 **125** 个公开组件

## 目录

- [NXApplication](#nxapplication)
- [NXTheme](#nxtheme)
- [NXWindow](#nxwindow)
- [NXWidget](#nxwidget)
- [NXAppBar](#nxappbar)
- [NXNavigationBar](#nxnavigationbar)
- [NXNavigationRouter](#nxnavigationrouter)
- [NXAcrylicUrlCard](#nxacrylicurlcard)
- [NXActionCommand](#nxactioncommand)
- [NXAutoComplete](#nxautocomplete)
- [NXBreadcrumbBar](#nxbreadcrumbbar)
- [NXCalendar](#nxcalendar)
- [NXCalendarPicker](#nxcalendarpicker)
- [NXCaptcha](#nxcaptcha)
- [NXChatBubble](#nxchatbubble)
- [NXCheckBox](#nxcheckbox)
- [NXCodeEditor](#nxcodeeditor)
- [NXColorDialog](#nxcolordialog)
- [NXComboBox](#nxcombobox)
- [NXCommandBar](#nxcommandbar)
- [NXContentDialog](#nxcontentdialog)
- [NXCopyButton](#nxcopybutton)
- [NXCountdown](#nxcountdown)
- [NXDashboardGauge](#nxdashboardgauge)
- [NXDialog](#nxdialog)
- [NXDivider](#nxdivider)
- [NXDockWidget](#nxdockwidget)
- [NXDoubleSpinBox](#nxdoublespinbox)
- [NXDrawerArea](#nxdrawerarea)
- [NXDropDownButton](#nxdropdownbutton)
- [NXDxgiManager](#nxdxgimanager)
- [NXEmojiPicker](#nxemojipicker)
- [NXEvent](#nxevent)
- [NXExpander](#nxexpander)
- [NXFloatButton](#nxfloatbutton)
- [NXFlowLayout](#nxflowlayout)
- [NXFlyout](#nxflyout)
- [NXGraphicsItem](#nxgraphicsitem)
- [NXGraphicsLineItem](#nxgraphicslineitem)
- [NXGraphicsScene](#nxgraphicsscene)
- [NXGraphicsView](#nxgraphicsview)
- [NXGroupBox](#nxgroupbox)
- [NXIconButton](#nxiconbutton)
- [NXImageCard](#nximagecard)
- [NXInfoBadge](#nxinfobadge)
- [NXInfoBar](#nxinfobar)
- [NXInputDialog](#nxinputdialog)
- [NXInteractiveCard](#nxinteractivecard)
- [NXKeyBinder](#nxkeybinder)
- [NXLCDNumber](#nxlcdnumber)
- [NXLineEdit](#nxlineedit)
- [NXListView](#nxlistview)
- [NXLog](#nxlog)
- [NXMarkdownViewer](#nxmarkdownviewer)
- [NXMenu](#nxmenu)
- [NXMenuBar](#nxmenubar)
- [NXMessageBar](#nxmessagebar)
- [NXMessageButton](#nxmessagebutton)
- [NXMessageDialog](#nxmessagedialog)
- [NXMultiSelectComboBox](#nxmultiselectcombobox)
- [NXNotificationCenter](#nxnotificationcenter)
- [NXNumberBox](#nxnumberbox)
- [NXPagination](#nxpagination)
- [NXPasswordBox](#nxpasswordbox)
- [NXPersonPicture](#nxpersonpicture)
- [NXPivot](#nxpivot)
- [NXPlainTextEdit](#nxplaintextedit)
- [NXPopconfirm](#nxpopconfirm)
- [NXPopularCard](#nxpopularcard)
- [NXProgressBar](#nxprogressbar)
- [NXProgressRing](#nxprogressring)
- [NXPromotionCard](#nxpromotioncard)
- [NXPromotionView](#nxpromotionview)
- [NXPushButton](#nxpushbutton)
- [NXQRCode](#nxqrcode)
- [NXRadioButton](#nxradiobutton)
- [NXRatingControl](#nxratingcontrol)
- [NXReminderCard](#nxremindercard)
- [NXRibbonBar](#nxribbonbar)
- [NXRibbonGroup](#nxribbongroup)
- [NXRibbonTabBar](#nxribbontabbar)
- [NXRoller](#nxroller)
- [NXRollerPicker](#nxrollerpicker)
- [NXRouter](#nxrouter)
- [NXScreenCaptureManager](#nxscreencapturemanager)
- [NXScrollArea](#nxscrollarea)
- [NXScrollBar](#nxscrollbar)
- [NXScrollPage](#nxscrollpage)
- [NXScrollPageArea](#nxscrollpagearea)
- [NXSelectorBar](#nxselectorbar)
- [NXSheetPanel](#nxsheetpanel)
- [NXSkeleton](#nxskeleton)
- [NXSlider](#nxslider)
- [NXSnackbar](#nxsnackbar)
- [NXSpinBox](#nxspinbox)
- [NXSplashScreen](#nxsplashscreen)
- [NXSplitButton](#nxsplitbutton)
- [NXSplitter](#nxsplitter)
- [NXSpotlight](#nxspotlight)
- [NXStatCard](#nxstatcard)
- [NXStatusBar](#nxstatusbar)
- [NXSteps](#nxsteps)
- [NXSuggestBox](#nxsuggestbox)
- [NXTabBar](#nxtabbar)
- [NXTabWidget](#nxtabwidget)
- [NXTableView](#nxtableview)
- [NXTableWidget](#nxtablewidget)
- [NXTag](#nxtag)
- [NXTeachingTip](#nxteachingtip)
- [NXTerminalWidget](#nxterminalwidget)
- [NXText](#nxtext)
- [NXTimeline](#nxtimeline)
- [NXToast](#nxtoast)
- [NXToggleButton](#nxtogglebutton)
- [NXToggleSwitch](#nxtoggleswitch)
- [NXToolBar](#nxtoolbar)
- [NXToolButton](#nxtoolbutton)
- [NXToolTip](#nxtooltip)
- [NXTransfer](#nxtransfer)
- [NXTreeSelect](#nxtreeselect)
- [NXTreeView](#nxtreeview)
- [NXUploadArea](#nxuploadarea)
- [NXVirtualList](#nxvirtuallist)
- [NXWatermark](#nxwatermark)
- [NXWizard](#nxwizard)

---

## NXApplication

**继承**: `QObject` | **头文件**: `NXApplication.h`

### 方法

- `void init()`
- `void syncWindowDisplayMode(QWidget *widget, bool isSync = true)`
- `static bool containsCursorToItem(QWidget *item)`

---

## NXTheme

**继承**: `QObject` | **头文件**: `NXTheme.h`

### 方法

- `void setThemeMode(NXThemeType::ThemeMode themeMode)`
- `NXThemeType::ThemeMode getThemeMode()`
- `void setThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor, const QColor &newColor)`
- `QColor getThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor)`

### 信号

- `themeModeChanged(NXThemeType::ThemeMode themeMode)`

---

## NXWindow

**继承**: `QMainWindow` | **头文件**: `NXWindow.h`

### 方法

- `void moveToCenter()`
- `QWidget * getCustomWidget(NXAppBarType::CustomArea customArea)`
- `void setCentralCustomWidget(QWidget *customWidget)`
- `QWidget * getCentralCustomWidget()`
- `void setCustomMenu(QMenu *customMenu)`
- `QMenu * getCustomMenu()`
- `void setUserInfoCardVisible(bool isVisible)`
- `void setUserInfoCardPixmap(const QPixmap &pix)`
- `void setUserInfoCardTitle(const QString &title)`
- `void setUserInfoCardSubTitle(const QString &subTitle)`
- `QString addExpanderNode(const QString &expanderTitle, NXIconType::IconName awesome = NXIconType::None)`
- `QString addCategoryNode(const QString &categoryTitle)`
- `NXNodeResultExpected addCategoryNode(const QString &categoryTitle, const QString &targetExpanderKey)`
- `void addCentralWidget(QWidget *centralWidget)`
- `QWidget * getCentralWidget(int index)`
- `bool getNavigationNodeIsExpanded(const QString &expanderKey)`
- `void expandNavigationNode(const QString &expanderKey)`
- `void collapseNavigationNode(const QString &expanderKey)`
- `void removeNavigationNode(const QString &nodeKey)`
- `int getPageOpenInNewWindowCount(const QString &nodeKey)`
- `void backtrackNavigationNode(const QString &nodeKey)`
- `void setNodeKeyPoints(const QString &nodeKey, int keyPoints)`
- `int getNodeKeyPoints(const QString &nodeKey)`
- `void setNavigationNodeTitle(const QString &nodeKey, const QString &nodeTitle)`
- `QString getNavigationNodeTitle(const QString &nodeKey)`
- `void navigation(const QString &pageKey)`
- `int getCurrentNavigationIndex()`
- `QString getCurrentNavigationPageKey()`
- `void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true)`
- `void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags)`
- `NXAppBarType::ButtonFlags getWindowButtonFlags()`
- `void setWindowMoviePath(NXThemeType::ThemeMode themeMode, const QString &moviePath)`
- `QString getWindowMoviePath(NXThemeType::ThemeMode themeMode)`
- `void setWindowPixmap(NXThemeType::ThemeMode themeMode, const QPixmap &pixmap)`
- `QPixmap getWindowPixmap(NXThemeType::ThemeMode themeMode)`
- `void setWindowMovieRate(qreal rate)`
- `qreal getWindowMovieRate()`
- `void tabifyDockWidget(QDockWidget *targetDockWidget, QDockWidget *dockWidget)`
- `void tabifyDockWidget(Qt::DockWidgetArea area, const QString &targetDockTitle, QDockWidget *dockWidget)`

### 信号

- `userInfoCardClicked()`
- `closeButtonClicked()`
- `navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, const QString &nodeKey)`
- `navigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, const QString &nodeKey)`
- `customWidgetChanged()`
- `centralCustomWidgetChanged()`
- `customMenuChanged()`
- `pageOpenInNewWindow(const QString &nodeKey)`

---

## NXWidget

**继承**: `QWidget` | **头文件**: `NXWidget.h`

### 方法

- `void moveToCenter()`
- `void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true)`
- `void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags)`
- `NXAppBarType::ButtonFlags getWindowButtonFlags()`
- `NXAppBar * getAppBar()`

### 信号

- `routeBackButtonClicked()`
- `navigationButtonClicked()`
- `themeChangeButtonClicked()`
- `closeButtonClicked()`

---

## NXAppBar

**继承**: `QWidget` | **头文件**: `NXAppBar.h`

### 方法

- `QWidget * getCustomWidget(NXAppBarType::CustomArea customArea)`
- `void setCustomMenu(QMenu *customMenu)`
- `QMenu * getCustomMenu()`
- `void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true)`
- `void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags)`
- `NXAppBarType::ButtonFlags getWindowButtonFlags()`
- `void setRouteBackButtonEnable(bool isEnable)`
- `void setRouteForwardButtonEnable(bool isEnable)`
- `int takeOverNativeEvent(const QByteArray &eventType, void *message, qintptr *result)`
- `int takeOverNativeEvent(const QByteArray &eventType, void *message, long *result)`

### 信号

- `routeBackButtonClicked()`
- `routeForwardButtonClicked()`
- `navigationButtonClicked()`
- `themeChangeButtonClicked()`
- `closeButtonClicked()`
- `customWidgetChanged()`
- `customMenuChanged()`

---

## NXNavigationBar

**继承**: `QWidget` | **头文件**: `NXNavigationBar.h`

### 方法

- `void setUserInfoCardVisible(bool isVisible)`
- `void setUserInfoCardPixmap(const QPixmap &pix)`
- `void setUserInfoCardTitle(const QString &title)`
- `void setUserInfoCardSubTitle(const QString &subTitle)`
- `void setIsNodeDragDropEnable(bool isEnable)`
- `void setToolTipOffset(int offsetX, int offsetY)`
- `QString addExpanderNode(const QString &expanderTitle, NXIconType::IconName awesome = NXIconType::None)`
- `QString addCategoryNode(const QString &categoryTitle)`
- `NXNodeResultExpected addCategoryNode(const QString &categoryTitle, const QString &targetExpanderKey)`
- `bool getNodeIsExpanded(const QString &expanderKey)`
- `void expandNode(const QString &expanderKey)`
- `void collapseNode(const QString &expanderKey)`
- `void removeNode(const QString &nodeKey)`
- `void setNodeKeyPoints(const QString &nodeKey, int keyPoints)`
- `int getNodeKeyPoints(const QString &nodeKey)`
- `void setNodeTitle(const QString &nodeKey, const QString &nodeTitle)`
- `QString getNodeTitle(const QString &nodeKey)`
- `void navigation(const QString &pageKey, bool isLogClicked = true, bool isRouteBack = false)`
- `void setDisplayMode(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true)`
- `NXNavigationType::NavigationDisplayMode getDisplayMode()`
- `int getPageOpenInNewWindowCount(const QString &nodeKey)`

### 信号

- `pageOpenInNewWindow(const QString &nodeKey)`
- `userInfoCardClicked()`
- `navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, const QString &nodeKey, bool isRouteBack)`
- `navigationNodeAdded(NXNavigationType::NavigationNodeType nodeType, const QString &nodeKey, QWidget *page)`
- `navigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, const QString &nodeKey)`
- `displayModeChanged(NXNavigationType::NavigationDisplayMode displayMode)`

---

## NXNavigationRouter

**继承**: `QObject` | **头文件**: `NXNavigationRouter.h`

### 方法

- `void clearNavigationRoute()`
- `void navigationRouteBack()`
- `void navigationRouteForward()`
- `void clearNavigationRoute(QObject *context)`
- `void navigationRouteBack(QObject *context)`
- `void navigationRouteForward(QObject *context)`

### 信号

- `navigationRouterStateChanged(NXNavigationRouterType::RouteMode routeMode)`
- `windowRouterStateChanged(QObject *context, NXNavigationRouterType::RouteMode routeMode)`

---

## NXAcrylicUrlCard

**继承**: `QPushButton` | **头文件**: `NXAcrylicUrlCard.h`

### 方法

- `void setCardPixmapSize(int width, int height)`

---

## NXActionCommand

**继承**: `QObject` | **头文件**: `NXActionCommander.h`

### 方法

- `void recordCommand(const QString &domainName, NXActionCommand *command, bool isRedo = true)`
- `void clearCommand(const QString &domainName)`
- `void undoCommand(const QString &domainName)`
- `void redoCommand(const QString &domainName)`
- `NXActionCommanderType::CommanderState getCommanderUndoState(const QString &domainName)`
- `NXActionCommanderType::CommanderState getCommanderRedoState(const QString &domainName)`

### 信号

- `commanderStateChanged(const QString &domainName, NXActionCommanderType::CommanderState state)`

---

## NXAutoComplete

**继承**: `QWidget` | **头文件**: `NXAutoComplete.h`

### 枚举

**MatchMode**: `Contains`, `StartsWith`, `EndsWith`, `RegExp`

### 方法

- `void setPlaceholderText(const QString &placeholderText)`
- `QString placeholderText()`
- `void setText(const QString &text)`
- `QString text()`
- `void setCompletions(const QStringList &completions)`
- `QStringList completions()`
- `void setMatchMode(MatchMode mode)`
- `MatchMode matchMode()`
- `void setFixedHeight(int h)`

### 信号

- `textChanged(const QString &text)`
- `textEdited(const QString &text)`
- `completionSelected(const QString &completion)`
- `returnPressed(const QString &text)`

---

## NXBreadcrumbBar

**继承**: `QWidget` | **头文件**: `NXBreadcrumbBar.h`

### 方法

- `void setBreadcrumbList(const QStringList &breadcrumbList)`
- `QStringList appendBreadcrumb(const QString &breadcrumb)`
- `QStringList removeBreadcrumb(const QString &breadcrumb)`
- `int getBreadcrumbListCount()`
- `QStringList getBreadcrumbList()`

### 信号

- `breadcrumbClicked(const QString &breadcrumb, const QStringList &lastBreadcrumbList)`

---

## NXCalendar

**继承**: `QWidget` | **头文件**: `NXCalendar.h`

### 信号

- `clicked(QDate date)`

---

## NXCalendarPicker

**继承**: `QPushButton` | **头文件**: `NXCalendarPicker.h`

### 信号

- `selectedDateChanged(QDate date)`

---

## NXCaptcha

**继承**: `QWidget` | **头文件**: `NXCaptcha.h`

### 枚举

**InputMode**: `DigitOnly`, `AlphaNumeric`

### 方法

- `void setInputMode(InputMode mode)`
- `InputMode getInputMode()`
- `QString getCode()`
- `void clear()`

### 信号

- `codeCompleted(const QString &code)`
- `codeChanged(const QString &code)`

---

## NXChatBubble

**继承**: `QWidget` | **头文件**: `NXChatBubble.h`

### 枚举

**BubbleDirection**: `Left`, `Right`

**MessageStatus**: `None`, `Sending`, `Sent`, `Read`, `Failed`

### 方法

- `void setDirection(BubbleDirection direction)`
- `BubbleDirection getDirection()`
- `void setStatus(MessageStatus status)`
- `MessageStatus getStatus()`
- `void setAvatar(const QPixmap &avatar)`
- `QPixmap getAvatar()`
- `void setBubbleColor(const QColor &color)`
- `QColor getBubbleColor()`
- `void setMessageImage(const QPixmap &image)`
- `QPixmap getMessageImage()`
- `void setImageMaxWidth(int width)`
- `int getImageMaxWidth()`

### 信号

- `imageDoubleClicked(const QPixmap &image)`

---

## NXCheckBox

**继承**: `QCheckBox` | **头文件**: `NXCheckBox.h`

---

## NXCodeEditor

**继承**: `QWidget` | **头文件**: `NXCodeEditor.h`

### 枚举

**Language**: `CPP`, `C`, `CSharp`, `Python`, `JavaScript`, `Lua`, `Rust`, `PHP`

### 方法

- `void setLanguage(Language lang)`
- `Language getLanguage()`

---

## NXColorDialog

**继承**: `QDialog` | **头文件**: `NXColorDialog.h`

### 方法

- `QColor getCustomColor(int index)`
- `QString getCurrent4ChannelColor()`

### 信号

- `colorSelected(const QColor &color)`

---

## NXComboBox

**继承**: `QComboBox` | **头文件**: `NXComboBox.h`

### 方法

- `void setEditable(bool editable)`

---

## NXCommandBar

**继承**: `QWidget` | **头文件**: `NXCommandBar.h`

### 结构体

**CommandItem**
```cpp
  NXIconType::IconName icon = NXIconType::None;
  QString text;
  bool isSeparator = false;
```

### 方法

- `void addItem(const CommandItem &item)`
- `void addSeparator()`
- `void clearItems()`

### 信号

- `itemClicked(int index)`

---

## NXContentDialog

**继承**: `QDialog` | **头文件**: `NXContentDialog.h`

### 方法

- `void setCentralWidget(QWidget *widget)`
- `NXPushButton * addButton(const QString &text)`
- `void removeButton(NXPushButton *button)`
- `void close()`
- `NXAppBar * appBar()`

### 信号

- `buttonClicked(NXPushButton *button)`

---

## NXCopyButton

**继承**: `QPushButton` | **头文件**: `NXCopyButton.h`

### 方法

- `void setText(const QString &text)`
- `void setNXIcon(NXIconType::IconName icon)`
- `void setSuccessIcon(NXIconType::IconName icon)`

### 信号

- `copyCompleted(const QString &text)`

---

## NXCountdown

**继承**: `QWidget` | **头文件**: `NXCountdown.h`

### 方法

- `void setTargetDateTime(const QDateTime &dateTime)`
- `QDateTime getTargetDateTime()`
- `void setRemainingSeconds(qint64 seconds)`
- `qint64 getRemainingSeconds()`
- `void start()`
- `void pause()`
- `void resume()`
- `void stop()`
- `bool isRunning()`

### 信号

- `timeout()`
- `tick(qint64 remainingSeconds)`

---

## NXDashboardGauge

**继承**: `QWidget` | **头文件**: `NXDashboardGauge.h`

### 方法

- `void setDangerPercent(qreal percent)`
- `qreal getDangerPercent()`
- `void setWarningPercent(qreal percent)`
- `qreal getWarningPercent()`
- `void setTickWarningPercent(qreal percent)`
- `qreal getTickWarningPercent()`

### 信号

- `valueChanged(qreal value)`

---

## NXDialog

**继承**: `QDialog` | **头文件**: `NXDialog.h`

### 方法

- `void moveToCenter()`
- `void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true)`
- `void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags)`
- `NXAppBarType::ButtonFlags getWindowButtonFlags()`

### 信号

- `routeBackButtonClicked()`
- `navigationButtonClicked()`
- `themeChangeButtonClicked()`
- `closeButtonClicked()`

---

## NXDivider

**继承**: `QWidget` | **头文件**: `NXDivider.h`

### 枚举

**ContentPositionType**: `Left`, `Center`, `Right`

---

## NXDockWidget

**继承**: `QDockWidget` | **头文件**: `NXDockWidget.h`

### 信号

- `dockClosed()`
- `dockResized(QSize size)`

---

## NXDoubleSpinBox

**继承**: `QDoubleSpinBox` | **头文件**: `NXDoubleSpinBox.h`

---

## NXDrawerArea

**继承**: `QWidget` | **头文件**: `NXDrawerArea.h`

### 方法

- `void setDrawerHeader(QWidget *widget)`
- `void addDrawer(QWidget *widget)`
- `void removeDrawer(QWidget *widget)`
- `void expand()`
- `void collapse()`
- `bool getIsExpand()`

### 信号

- `expandStateChanged(bool isExpand)`

---

## NXDropDownButton

**继承**: `QWidget` | **头文件**: `NXDropDownButton.h`

### 方法

- `void setMenu(NXMenu *menu)`
- `NXMenu * getMenu()`

---

## NXDxgiManager

**继承**: `QObject` | **头文件**: `NXDxgiManager.h`

### 方法

- `QStringList getDxDeviceList()`
- `QStringList getOutputDeviceList()`
- `QImage grabScreenToImage()`
- `void startGrabScreen()`
- `void stopGrabScreen()`
- `bool getIsGrabScreen()`
- `bool setDxDeviceID(int dxID)`
- `int getDxDeviceID()`
- `bool setOutputDeviceID(int deviceID)`
- `int getOutputDeviceID()`
- `void setGrabArea(int width, int height)`
- `void setGrabArea(int x, int y, int width, int height)`
- `QRect getGrabArea()`
- `void setGrabFrameRate(int frameRateValue)`
- `int getGrabFrameRate()`
- `void setTimeoutMsValue(int timeoutValue)`
- `int getTimeoutMsValue()`
- `explicit NXDxgiScreen(QWidget *parent = nullptr)`
- `void setIsSyncGrabSize(bool isSyncGrabSize)`
- `bool getIsSyncGrabSize()`

### 信号

- `grabImageUpdate(const QImage &img)`

---

## NXEmojiPicker

**继承**: `QWidget` | **头文件**: `NXEmojiPicker.h`

### 方法

- `void popup(QWidget *anchor)`
- `void popup(QPoint pos)`

### 信号

- `emojiSelected(const QString &emoji)`

---

## NXEvent

**继承**: `QObject` | **头文件**: `NXEventBus.h`

### 方法

- `NXEventBusType::EventBusReturnType registerAndInit()`
- `NXEventBusType::EventBusReturnType post(const QString &eventName, const QVariantMap &data = {})`
- `QStringList getRegisteredEventsName()`

---

## NXExpander

**继承**: `QWidget` | **头文件**: `NXExpander.h`

### 枚举

**ExpandDirection**: `Down`, `Up`

### 方法

- `void setExpandDirection(ExpandDirection direction)`
- `ExpandDirection getExpandDirection()`
- `void setContentWidget(QWidget *widget)`
- `QWidget * getContentWidget()`
- `void setHeaderWidget(QWidget *widget)`
- `void setIsExpanded(bool expanded)`
- `bool getIsExpanded()`

### 信号

- `expandStateChanged(bool expanded)`

---

## NXFloatButton

**继承**: `QWidget` | **头文件**: `NXFloatButton.h`

### 枚举

**Position**: `BottomRight`, `BottomLeft`, `TopRight`, `TopLeft`

### 方法

- `void setIcon(NXIconType::IconName icon)`
- `NXIconType::IconName getIcon()`
- `void setPosition(Position position)`
- `Position getPosition()`
- `void setMenu(NXMenu *menu)`
- `NXMenu * getMenu()`

### 信号

- `clicked()`

---

## NXFlowLayout

**继承**: `QLayout` | **头文件**: `NXFlowLayout.h`

### 方法

- `int horizontalSpacing()`
- `int verticalSpacing()`
- `void setIsAnimation(bool isAnimation)`

---

## NXFlyout

**继承**: `QWidget` | **头文件**: `NXFlyout.h`

### 方法

- `void setContentWidget(QWidget *widget)`
- `void showFlyout(QWidget *target)`
- `void closeFlyout()`

### 信号

- `closed()`

---

## NXGraphicsItem

**继承**: `QGraphicsObject` | **头文件**: `NXGraphicsItem.h`

### 方法

- `QString getItemUID()`
- `void setLinkPortState(bool isFullLink)`
- `void setLinkPortState(bool isLink, int portIndex)`
- `bool getLinkPortState(int portIndex)`
- `int getUsedLinkPortCount()`
- `int getUnusedLinkPortCount()`

---

## NXGraphicsLineItem

**继承**: `QGraphicsPathItem` | **头文件**: `NXGraphicsLineItem.h`

### 方法

- `bool isTargetLink(NXGraphicsItem *item)`
- `bool isTargetLink(NXGraphicsItem *item1, NXGraphicsItem *item2)`
- `bool isTargetLink(NXGraphicsItem *item1, NXGraphicsItem *item2, int port1, int port2)`

---

## NXGraphicsScene

**继承**: `QGraphicsScene` | **头文件**: `NXGraphicsScene.h`

### 方法

- `void addItem(NXGraphicsItem *item)`
- `void removeItem(NXGraphicsItem *item)`
- `void removeSelectedItems()`
- `void clear()`
- `void setSceneMode(NXGraphicsSceneType::SceneMode mode)`
- `NXGraphicsSceneType::SceneMode getSceneMode()`
- `void selectAllItems()`
- `bool addItemLink(NXGraphicsItem *item1, NXGraphicsItem *item2, int port1 = 0, int port2 = 0)`
- `bool removeItemLink(NXGraphicsItem *item1)`
- `bool removeItemLink(NXGraphicsItem *item1, NXGraphicsItem *item2, int port1 = 0, int port2 = 0)`
- `void serialize()`
- `void deserialize()`

### 信号

- `showItemLink()`
- `mouseLeftClickedItem(NXGraphicsItem *item)`
- `mouseRightClickedItem(NXGraphicsItem *item)`
- `mouseDoubleClickedItem(NXGraphicsItem *item)`

---

## NXGraphicsView

**继承**: `QGraphicsView` | **头文件**: `NXGraphicsView.h`

---

## NXGroupBox

**继承**: `QGroupBox` | **头文件**: `NXGroupBox.h`

---

## NXIconButton

**继承**: `QPushButton` | **头文件**: `NXIconButton.h`

### 方法

- `void setAwesome(NXIconType::IconName awesome)`
- `NXIconType::IconName getAwesome()`
- `void setPixmap(const QPixmap &pix)`

---

## NXImageCard

**继承**: `QWidget` | **头文件**: `NXImageCard.h`

---

## NXInfoBadge

**继承**: `QWidget` | **头文件**: `NXInfoBadge.h`

### 枚举

**BadgeMode**: `Dot`, `Value_`, `Icon`

**Severity**: `Attention`, `Informational`, `Success`, `Caution`, `Critical`

### 方法

- `void setBadgeMode(BadgeMode mode)`
- `BadgeMode getBadgeMode()`
- `void setMaxValue(int maxValue)`
- `int getMaxValue()`
- `void setSeverity(Severity severity)`
- `Severity getSeverity()`
- `void attachTo(QWidget *target)`

---

## NXInfoBar

**继承**: `QFrame` | **头文件**: `NXInfoBar.h`

### 方法

- `void clearActions()`
- `void closeInfoBar()`

### 信号

- `closed()`
- `closeButtonClicked()`

---

## NXInputDialog

**继承**: `QDialog` | **头文件**: `NXInputDialog.h`

### 方法

- `void setTextEchoMode(QLineEdit::EchoMode mode)`
- `QLineEdit::EchoMode textEchoMode()`
- `void setIntRange(int minValue, int maxValue, int step = 1)`
- `void setDoubleRange(double minValue, double maxValue, int decimals = 2)`
- `void setMultiLine(bool multiLine)`

### 信号

- `textValueChanged(const QString &text)`
- `intValueChanged(int value)`
- `doubleValueChanged(double value)`

---

## NXInteractiveCard

**继承**: `QPushButton` | **头文件**: `NXInteractiveCard.h`

### 方法

- `void setCardPixmapSize(int width, int height)`

---

## NXKeyBinder

**继承**: `QLabel` | **头文件**: `NXKeyBinder.h`

### 信号

- `binderKeyTextChanged(const QString &binderKeyText)`
- `nativeVirtualBinderKeyChanged(quint32 binderKey)`

---

## NXLCDNumber

**继承**: `QLCDNumber` | **头文件**: `NXLCDNumber.h`

---

## NXLineEdit

**继承**: `QLineEdit` | **头文件**: `NXLineEdit.h`

### 信号

- `focusIn(const QString &text)`
- `focusOut(const QString &text)`
- `wmFocusOut(const QString &text)`

---

## NXListView

**继承**: `QListView` | **头文件**: `NXListView.h`

---

## NXLog

**继承**: `QObject` | **头文件**: `NXLog.h`

### 方法

- `void initMessageLog(bool isEnable)`

### 信号

- `logMessage(const QString &log)`

---

## NXMarkdownViewer

**继承**: `QWidget` | **头文件**: `NXMarkdownViewer.h`

---

## NXMenu

**继承**: `QMenu` | **头文件**: `NXMenu.h`

### 方法

- `QAction * addMenu(QMenu *menu)`
- `NXMenu * addMenu(const QString &title)`
- `NXMenu * addMenu(const QIcon &icon, const QString &title)`
- `NXMenu * addMenu(NXIconType::IconName icon, const QString &title)`
- `QAction * addNXIconAction(NXIconType::IconName icon, const QString &text)`
- `QAction * addNXIconAction(NXIconType::IconName icon, const QString &text, const QKeySequence &shortcut)`
- `bool isHasParentMenu()`
- `bool isHasChildMenu()`
- `bool isHasIcon()`

### 信号

- `menuShow()`

---

## NXMenuBar

**继承**: `QMenuBar` | **头文件**: `NXMenuBar.h`

### 方法

- `QAction * addMenu(QMenu *menu)`
- `NXMenu * addMenu(const QString &title)`
- `NXMenu * addMenu(const QIcon &icon, const QString &title)`
- `NXMenu * addMenu(NXIconType::IconName, const QString &title)`
- `QAction * addNXIconAction(NXIconType::IconName icon, const QString &text)`
- `QAction * addNXIconAction(NXIconType::IconName icon, const QString &text, const QKeySequence &shortcut)`

---

## NXMessageBar

**继承**: `QWidget` | **头文件**: `NXMessageBar.h`

---

## NXMessageButton

**继承**: `QPushButton` | **头文件**: `NXMessageButton.h`

---

## NXMessageDialog

**继承**: `QWidget` | **头文件**: `NXMessageDialog.h`

### 信号

- `confirmed()`
- `cancelled()`

---

## NXMultiSelectComboBox

**继承**: `QComboBox` | **头文件**: `NXMultiSelectComboBox.h`

### 方法

- `void setCurrentSelection(const QString &selection)`
- `void setCurrentSelection(const QStringList &selection)`
- `void setCurrentSelection(int index)`
- `void setCurrentSelection(const QList<int> &selectionIndex)`
- `QStringList getCurrentSelection()`

### 信号

- `itemSelectionChanged(const QVector<bool> &itemSelection)`
- `currentTextListChanged(const QStringList &selectedTextList)`

---

## NXNotificationCenter

**继承**: `QWidget` | **头文件**: `NXNotificationCenter.h`

### 结构体

**NotificationItem**
```cpp
  QString title;
  QString content;
  QString timestamp;
  NXIconType::IconName icon = NXIconType::None;
```

### 方法

- `void addNotification(const NotificationItem &item)`
- `void clearAll()`
- `int getNotificationCount()`
- `void showPanel(QWidget *anchor)`
- `void hidePanel()`
- `bool isPanelVisible()`

### 信号

- `notificationClicked(int index)`
- `panelVisibilityChanged(bool visible)`

---

## NXNumberBox

**继承**: `QWidget` | **头文件**: `NXNumberBox.h`

### 方法

- `void stepUp()`
- `void stepDown()`

### 信号

- `valueChanged(double value)`

---

## NXPagination

**继承**: `QWidget` | **头文件**: `NXPagination.h`

### 信号

- `currentPageChanged(int page)`

---

## NXPasswordBox

**继承**: `QLineEdit` | **头文件**: `NXPasswordBox.h`

### 信号

- `focusIn(const QString &text)`
- `focusOut(const QString &text)`
- `wmFocusOut(const QString &text)`

---

## NXPersonPicture

**继承**: `QWidget` | **头文件**: `NXPersonPicture.h`

---

## NXPivot

**继承**: `QWidget` | **头文件**: `NXPivot.h`

### 方法

- `void appendPivot(const QString &pivotTitle)`
- `void removePivot(const QString &pivotTitle)`

### 信号

- `pivotClicked(int index)`
- `pivotDoubleClicked(int index)`

---

## NXPlainTextEdit

**继承**: `QPlainTextEdit` | **头文件**: `NXPlainTextEdit.h`

---

## NXPopconfirm

**继承**: `QWidget` | **头文件**: `NXPopconfirm.h`

### 方法

- `void showPopconfirm(QWidget *target)`
- `void closePopconfirm()`

### 信号

- `confirmed()`
- `cancelled()`
- `closed()`

---

## NXPopularCard

**继承**: `QWidget` | **头文件**: `NXPopularCard.h`

### 信号

- `popularCardClicked()`
- `popularCardButtonClicked()`

---

## NXProgressBar

**继承**: `QProgressBar` | **头文件**: `NXProgressBar.h`

### 方法

- `void setMinimum(int minimum)`
- `void setMaximum(int maximum)`

---

## NXProgressRing

**继承**: `QWidget` | **头文件**: `NXProgressRing.h`

### 方法

- `void setRange(int min, int max)`

### 信号

- `rangeChanged(int min, int max)`

---

## NXPromotionCard

**继承**: `QWidget` | **头文件**: `NXPromotionCard.h`

### 信号

- `promotionCardClicked()`

---

## NXPromotionView

**继承**: `QWidget` | **头文件**: `NXPromotionView.h`

### 方法

- `void appendPromotionCard(NXPromotionCard *card)`

---

## NXPushButton

**继承**: `QPushButton` | **头文件**: `NXPushButton.h`

### 方法

- `void setNXIcon(NXIconType::IconName icon)`
- `void setNXIcon(NXIconType::IconName icon, int iconSize)`

---

## NXQRCode

**继承**: `QWidget` | **头文件**: `NXQRCode.h`

### 枚举

**ErrorCorrectionLevel**: `Low`, `Medium`, `Quartile`, `High`

### 方法

- `void setText(const QString &text)`
- `QString getText()`
- `void setErrorCorrectionLevel(ErrorCorrectionLevel level)`
- `ErrorCorrectionLevel getErrorCorrectionLevel()`
- `QPixmap toPixmap(int size = 256)`

---

## NXRadioButton

**继承**: `QRadioButton` | **头文件**: `NXRadioButton.h`

---

## NXRatingControl

**继承**: `QWidget` | **头文件**: `NXRatingControl.h`

### 信号

- `ratingChanged(int rating)`

---

## NXReminderCard

**继承**: `QPushButton` | **头文件**: `NXReminderCard.h`

### 方法

- `void setCardPixmapSize(int width, int height)`

---

## NXRibbonBar

**继承**: `QWidget` | **头文件**: `NXRibbonBar.h`

### 方法

- `void bindTabBar(NXRibbonTabBar *tabBar)`
- `NXRibbonTabBar * tabBar()`
- `QWidget * addTab(const QString &title)`
- `NXRibbonGroup * addGroup(QWidget *page, const QString &title)`
- `int tabCount()`
- `QString tabText(int index)`
- `void setCollapsed(bool collapsed)`
- `bool isCollapsed()`
- `void setPinned(bool pinned)`
- `bool isPinned()`
- `void setAnimationDuration(int durationMs)`
- `int getAnimationDuration()`
- `void showPinContextMenu(QPoint globalPos)`

### 信号

- `tabClicked(int index)`
- `collapsedChanged(bool collapsed)`
- `pinnedChanged(bool pinned)`

---

## NXRibbonGroup

**继承**: `QWidget` | **头文件**: `NXRibbonGroup.h`

### 枚举

**ButtonSize**: `Large`, `Small`

### 方法

- `void setTitle(const QString &title)`
- `QString getTitle()`
- `NXToolButton * addToolButton(NXIconType::IconName icon, const QString &text, ButtonSize size = Large)`
- `void addWidget(QWidget *widget)`

---

## NXRibbonTabBar

**继承**: `QWidget` | **头文件**: `NXRibbonTabBar.h`

### 方法

- `int appendTab(const QString &title)`
- `void removeTab(int index)`
- `void clear()`
- `int tabCount()`
- `QString tabText(int index)`
- `void setTabText(int index, const QString &title)`

### 信号

- `tabClicked(int index)`
- `tabReclicked(int index)`

---

## NXRoller

**继承**: `QWidget` | **头文件**: `NXRoller.h`

### 方法

- `void setCurrentData(const QString &data)`
- `QString getCurrentData()`

### 信号

- `currentDataChanged(const QString &data)`

---

## NXRollerPicker

**继承**: `QPushButton` | **头文件**: `NXRollerPicker.h`

### 方法

- `void addRoller(const QStringList &itemList, bool isEnableLoop = true)`
- `void removeRoller(int index)`
- `void setRollerItemList(int index, const QStringList &itemList)`
- `void setRollerWidth(int index, int width)`
- `void setCurrentData(int index, const QString &data)`
- `void setCurrentData(const QStringList &dataList)`
- `QString getCurrentData(int index)`
- `QStringList getCurrentData()`
- `void setCurrentIndex(int rollerIndex, int index)`
- `void setCurrentIndex(const QList<int> &indexList)`
- `int getCurrentIndex(int rollerIndex)`

### 信号

- `currentDataChanged(const QStringList &dataList)`
- `currentDataSelectionChanged(const QStringList &dataList)`

---

## NXRouter

**继承**: `QObject` | **头文件**: `NXRouter.h`

### 方法

- `void bindWindow(NXWindow *window)`
- `NXWindow * getBoundWindow()`
- `NXRouterType::NavigationResult addRoute(const NXRouteConfig &config)`
- `NXRouterType::NavigationResult addRoutes(const QVector<NXRouteConfig> &configs)`
- `NXRouterType::NavigationResult addDynamicRoute(const QString &parentPath, const NXRouteConfig &config)`
- `NXRouterType::NavigationResult removeRoute(const QString &path)`
- `bool hasRoute(const QString &path)`
- `QStringList getRoutePaths()`
- `QVariantMap getRouteMeta(const QString &path)`
- `NXRouterType::NavigationResult push(const QString &path, const QVariantMap &params = {})`
- `NXRouterType::NavigationResult replace(const QString &path, const QVariantMap &params = {})`
- `void back()`
- `void forward()`
- `QString getCurrentPath()`
- `QVariantMap getCurrentParams()`
- `int beforeEach(const NXRouteGuard &guard)`
- `int afterEach(const NXRouteAfterHook &hook)`
- `void removeBeforeGuard(int guardId)`
- `void removeAfterHook(int hookId)`
- `void setRouteBeforeEnter(const QString &path, const NXRouteGuard &guard)`
- `void installRoutes()`
- `void resetRouter()`

### 信号

- `routeChanged(const QString &path, const QVariantMap &params)`
- `navigationBlocked(const QString &path)`
- `routeTableChanged()`

---

## NXScreenCaptureManager

**继承**: `QObject` | **头文件**: `NXScreenCaptureManager.h`

### 方法

- `QStringList getDisplayList()`
- `QImage grabScreenToImage()`
- `void startGrabScreen()`
- `void stopGrabScreen()`
- `bool getIsGrabScreen()`
- `bool setDisplayID(int displayID)`
- `int getDisplayID()`
- `void setGrabArea(int width, int height)`
- `void setGrabArea(int x, int y, int width, int height)`
- `QRect getGrabArea()`
- `void setGrabFrameRate(int frameRateValue)`
- `int getGrabFrameRate()`
- `explicit NXScreenCaptureScreen(QWidget *parent = nullptr)`
- `void setIsSyncGrabSize(bool isSyncGrabSize)`
- `bool getIsSyncGrabSize()`

### 信号

- `grabImageUpdate(const QImage &img)`

---

## NXScrollArea

**继承**: `QScrollArea` | **头文件**: `NXScrollArea.h`

### 方法

- `void setIsGrabGesture(bool isEnable, qreal mousePressEventDelay = 0.5)`
- `void setIsOverShoot(Qt::Orientation orientation, bool isEnable)`
- `bool getIsOverShoot(Qt::Orientation orientation)`
- `void setIsAnimation(Qt::Orientation orientation, bool isAnimation)`
- `bool getIsAnimation(Qt::Orientation orientation)`

---

## NXScrollBar

**继承**: `QScrollBar` | **头文件**: `NXScrollBar.h`

### 信号

- `rangeAnimationFinished()`

---

## NXScrollPage

**继承**: `QWidget` | **头文件**: `NXScrollPage.h`

### 方法

- `void setPageTitle(const QString &title)`
- `void navigation(int widgetIndex, bool isLogRoute = true)`
- `void setPageTitleSpacing(int spacing)`
- `int getPageTitleSpacing()`
- `void setTitleVisible(bool isVisible)`

---

## NXScrollPageArea

**继承**: `QWidget` | **头文件**: `NXScrollPageArea.h`

---

## NXSelectorBar

**继承**: `QWidget` | **头文件**: `NXSelectorBar.h`

### 方法

- `void addItem(const QString &text)`
- `void addItem(NXIconType::IconName icon, const QString &text)`
- `void clearItems()`
- `int getItemCount()`

### 信号

- `currentIndexChanged(int index)`

---

## NXSheetPanel

**继承**: `QWidget` | **头文件**: `NXSheetPanel.h`

### 方法

- `void setCentralWidget(QWidget *widget)`
- `void open(NXSheetPanelType::DetentLevel level = NXSheetPanelType::Half)`
- `void close()`
- `NXSheetPanelType::DetentLevel currentDetent()`
- `bool isOpened()`

### 信号

- `opened()`
- `closed()`
- `detentChanged(NXSheetPanelType::DetentLevel level)`

---

## NXSkeleton

**继承**: `QWidget` | **头文件**: `NXSkeleton.h`

### 枚举

**SkeletonType**: `Text`, `Circle`, `Rectangle`

### 方法

- `void setSkeletonType(SkeletonType type)`
- `SkeletonType getSkeletonType()`

---

## NXSlider

**继承**: `QSlider` | **头文件**: `NXSlider.h`

---

## NXSnackbar

**继承**: `QWidget` | **头文件**: `NXSnackbar.h`

### 枚举

**SnackbarType**: `Success`, `Info`, `Warning`, `Error`

### 方法

- `void dismiss()`
- `static void setMaxCount(int count)`
- `static int getMaxCount()`

### 信号

- `actionClicked()`
- `closed()`

---

## NXSpinBox

**继承**: `QSpinBox` | **头文件**: `NXSpinBox.h`

---

## NXSplashScreen

**继承**: `QWidget` | **头文件**: `NXSplashScreen.h`

### 方法

- `void setLogo(const QPixmap &logo)`
- `void setTitle(const QString &title)`
- `void setSubTitle(const QString &subTitle)`
- `void setStatusText(const QString &text)`
- `void show()`
- `void close()`
- `void finish(QWidget *mainWindow)`

### 信号

- `closed()`

---

## NXSplitButton

**继承**: `QWidget` | **头文件**: `NXSplitButton.h`

### 方法

- `void setMenu(NXMenu *menu)`
- `NXMenu * getMenu()`

### 信号

- `clicked()`

---

## NXSplitter

**继承**: `QSplitter` | **头文件**: `NXSplitter.h`

---

## NXSpotlight

**继承**: `QWidget` | **头文件**: `NXSpotlight.h`

### 结构体

**SpotlightStep**
```cpp
  QWidget *target { nullptr;
```

### 方法

- `void setSteps(const QList<SpotlightStep> &steps)`
- `void start()`
- `void next()`
- `void previous()`
- `void finish()`
- `int currentStep()`
- `int stepCount()`

### 信号

- `stepChanged(int step)`
- `finished()`

---

## NXStatCard

**继承**: `QWidget` | **头文件**: `NXStatCard.h`

### 枚举

**TrendType**: `None`, `Up`, `Down`, `Neutral`

### 方法

- `void setTrend(TrendType trend)`
- `TrendType getTrend()`
- `void setTrendText(const QString &text)`
- `QString getTrendText()`

---

## NXStatusBar

**继承**: `QStatusBar` | **头文件**: `NXStatusBar.h`

---

## NXSteps

**继承**: `QWidget` | **头文件**: `NXSteps.h`

### 方法

- `void setStepTitles(const QStringList &titles)`
- `QStringList getStepTitles()`
- `void next()`
- `void previous()`

### 信号

- `currentStepChanged(int step)`

---

## NXSuggestBox

**继承**: `QWidget` | **头文件**: `NXSuggestBox.h`

### 方法

- `void setPlaceholderText(const QString &placeholderText)`
- `void setFixedSize(QSize size)`
- `void setFixedSize(int w, int h)`
- `void setFixedHeight(int h)`
- `explicit SuggestData()`
- `explicit SuggestData(NXIconType::IconName icon, const QString &suggestText, const QVariantMap &suggestData = {})`
- `QString addSuggestion(const QString &suggestText, const QVariantMap &suggestData = {})`
- `QString addSuggestion(NXIconType::IconName icon, const QString &suggestText, const QVariantMap &suggestData = {})`
- `QString addSuggestion(const NXSuggestBox::SuggestData &suggestData)`
- `QStringList addSuggestion(const QList<NXSuggestBox::SuggestData> &suggestDataList)`
- `void removeSuggestion(const QString &suggestKey)`
- `void removeSuggestion(int index)`
- `void clearSuggestion()`

### 信号

- `suggestionClicked(const NXSuggestBox::SuggestData &suggestData)`

---

## NXTabBar

**继承**: `QTabBar` | **头文件**: `NXTabBar.h`

### 信号

- `tabDragCreate(QMimeData *mimeData)`
- `tabDragEnter(QMimeData *mimeData)`
- `tabDragLeave(QMimeData *mimeData)`
- `tabDragDrop(QMimeData *mimeData)`

---

## NXTabWidget

**继承**: `QTabWidget` | **头文件**: `NXTabWidget.h`

### 方法

- `void setTabPosition(TabPosition position)`

---

## NXTableView

**继承**: `QTableView` | **头文件**: `NXTableView.h`

### 方法

- `void setColumnPadding(int column, int padding)`
- `int columnPadding(int column)`
- `void clearColumnPadding(int column)`

### 信号

- `tableViewShow()`
- `tableViewHide()`
- `hoverIndexChanged(const QModelIndex &index)`

---

## NXTableWidget

**继承**: `QTableWidget` | **头文件**: `NXTableWidget.h`

### 方法

- `void insertRows(int row, int count)`
- `void removeRows(int row, int count)`
- `void insertColumns(int column, int count)`
- `void removeColumns(int column, int count)`
- `void setItemText(int row, int column, const QString &text)`
- `QString getItemText(int row, int column)`
- `void setRowData(int row, const QStringList &data)`
- `QStringList getRowData(int row)`

### 信号

- `tableWidgetShow()`
- `tableWidgetHide()`

---

## NXTag

**继承**: `QWidget` | **头文件**: `NXTag.h`

### 枚举

**TagColor**: `Default`, `Primary`, `Success`, `Warning`, `Danger`

### 方法

- `void setTagColor(TagColor color)`
- `TagColor getTagColor()`

### 信号

- `closed()`
- `clicked()`
- `checkedChanged(bool checked)`

---

## NXTeachingTip

**继承**: `QWidget` | **头文件**: `NXTeachingTip.h`

### 枚举

**TailPosition**: `Auto`, `Top`, `Bottom`, `Left`, `Right`

### 方法

- `void setTailPosition(TailPosition position)`
- `TailPosition getTailPosition()`
- `void setTarget(QWidget *target)`
- `QWidget * getTarget()`
- `void setCloseButtonVisible(bool visible)`
- `void clearActions()`
- `void showTip()`
- `void closeTip()`

### 信号

- `closed()`
- `closeButtonClicked()`

---

## NXTerminalWidget

**继承**: `QWidget` | **头文件**: `NXTerminalWidget.h`

### 方法

- `void appendHtml(const QString &html)`
- `void appendError(const QString &text)`
- `void appendSuccess(const QString &text)`
- `void clear()`
- `QStringList getCommandHistory()`

### 信号

- `commandSubmitted(const QString &command)`
- `tabPressed(const QString &currentInput)`

---

## NXText

**继承**: `QLabel` | **头文件**: `NXText.h`

### 信号

- `clicked()`

---

## NXTimeline

**继承**: `QWidget` | **头文件**: `NXTimeline.h`

### 结构体

**TimelineItem**
```cpp
  QString title;
  QString content;
  QString timestamp;
  NXIconType::IconName icon = NXIconType::None;
```

### 方法

- `void addItem(const TimelineItem &item)`
- `void clearItems()`
- `int getItemCount()`

---

## NXToast

**继承**: `QWidget` | **头文件**: `NXToast.h`

### 枚举

**ToastType**: `Success`, `Info`, `Warning`, `Error`

### 方法

- `static void success(const QString &text, int displayMsec = 2000, QWidget *parent = nullptr)`
- `static void info(const QString &text, int displayMsec = 2000, QWidget *parent = nullptr)`
- `static void warning(const QString &text, int displayMsec = 2000, QWidget *parent = nullptr)`
- `static void error(const QString &text, int displayMsec = 2000, QWidget *parent = nullptr)`

---

## NXToggleButton

**继承**: `QWidget` | **头文件**: `NXToggleButton.h`

### 方法

- `void setNXIcon(NXIconType::IconName icon)`
- `void setIsToggled(bool isToggled)`
- `bool getIsToggled()`

### 信号

- `toggled(bool checked)`

---

## NXToggleSwitch

**继承**: `QWidget` | **头文件**: `NXToggleSwitch.h`

### 方法

- `void setIsToggled(bool isToggled)`
- `bool getIsToggled()`

### 信号

- `toggled(bool checked)`

---

## NXToolBar

**继承**: `QToolBar` | **头文件**: `NXToolBar.h`

### 方法

- `void setToolBarSpacing(int spacing)`
- `int getToolBarSpacing()`
- `void setToolButtonSize(QSize size)`
- `QSize getToolButtonSize()`
- `QAction * addNXIconAction(NXIconType::IconName icon, const QString &text)`
- `QAction * addNXIconAction(NXIconType::IconName icon, const QString &text, const QKeySequence &shortcut)`

---

## NXToolButton

**继承**: `QToolButton` | **头文件**: `NXToolButton.h`

### 方法

- `void setIsTransparent(bool isTransparent)`
- `bool getIsTransparent()`
- `void setMenu(NXMenu *menu)`
- `void setNXIcon(NXIconType::IconName icon)`
- `void setNXIcon(NXIconType::IconName icon, int rotate)`

---

## NXToolTip

**继承**: `QWidget` | **头文件**: `NXToolTip.h`

### 方法

- `void updatePos()`

---

## NXTransfer

**继承**: `QWidget` | **头文件**: `NXTransfer.h`

### 方法

- `void setSourceItems(const QStringList &items)`
- `void addSourceItem(const QString &text)`
- `void addSourceItems(const QStringList &items)`
- `QStringList getSourceItems()`
- `QStringList getTargetItems()`
- `void moveToTarget()`
- `void moveToSource()`
- `void moveAllToTarget()`
- `void moveAllToSource()`

### 信号

- `transferChanged(const QStringList &sourceItems, const QStringList &targetItems)`

---

## NXTreeSelect

**继承**: `QWidget` | **头文件**: `NXTreeSelect.h`

### 方法

- `void setModel(QStandardItemModel *model)`
- `QStandardItemModel * model()`
- `void setCurrentIndex(const QModelIndex &index)`
- `QModelIndex currentIndex()`
- `QString currentText()`
- `void expandAll()`
- `void collapseAll()`

### 信号

- `currentIndexChanged(const QModelIndex &index)`
- `currentTextChanged(const QString &text)`

---

## NXTreeView

**继承**: `QTreeView` | **头文件**: `NXTreeView.h`

### 方法

- `void setNXIcon(NXIconType::IconName icon)`

---

## NXUploadArea

**继承**: `QWidget` | **头文件**: `NXUploadArea.h`

### 方法

- `QStringList getSelectedFiles()`
- `void clearFiles()`
- `void setAcceptedMimeFilter(const QString &filter)`
- `QString getAcceptedMimeFilter()`

### 信号

- `filesSelected(const QStringList &filePaths)`
- `fileAdded(const QString &filePath)`
- `fileRemoved(const QString &filePath)`
- `fileRejected(const QString &filePath, const QString &reason)`

---

## NXVirtualList

**继承**: `QListView` | **头文件**: `NXVirtualList.h`

### 方法

- `void setItemCount(int count)`
- `int getItemCount()`

### 信号

- `itemRequestData(int startIndex, int endIndex)`

---

## NXWatermark

**继承**: `QWidget` | **头文件**: `NXWatermark.h`

---

## NXWizard

**继承**: `QWidget` | **头文件**: `NXWizard.h`

### 方法

- `void addStep(const QString &title, QWidget *page)`
- `void next()`
- `void previous()`
- `void finish()`
- `int getStepCount()`

### 信号

- `currentStepChanged(int step)`
- `finished()`
- `cancelled()`

---
