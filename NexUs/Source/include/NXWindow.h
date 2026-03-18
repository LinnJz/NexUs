#ifndef NXWINDOW_H
#define NXWINDOW_H

#include <QMainWindow>
#include "NXAppBar.h"
#include "NXSuggestBox.h"
class NXWindowPrivate;

class NX_EXPORT NXWindow : public QMainWindow
{
  Q_OBJECT
  Q_Q_CREATE(NXWindow)
  Q_PROPERTY_CREATE_H(bool, IsStayTop)
  Q_PROPERTY_CREATE_H(bool, IsFixedSize)
  Q_PROPERTY_CREATE_H(bool, IsDefaultClosed)
  Q_PROPERTY_CREATE_H(bool, IsCentralStackedWidgetTransparent)
  Q_PROPERTY_CREATE_H(bool, IsAllowPageOpenInNewWindow)
  Q_PROPERTY_CREATE_H(bool, IsNavigationBarEnable)
  Q_PROPERTY_CREATE_H(int, NavigationBarWidth)
  Q_PROPERTY_CREATE_H(int, CurrentStackIndex)
  Q_PROPERTY_CREATE_H(int, AppBarHeight)
  Q_PROPERTY_CREATE_H(int, ThemeChangeTime)
  Q_PROPERTY_CREATE_H(NXNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
  Q_PROPERTY_CREATE_H(NXWindowType::StackSwitchMode, StackSwitchMode)
  Q_PROPERTY_CREATE_H(NXWindowType::PaintMode, WindowPaintMode)
  Q_TAKEOVER_NATIVEEVENT_H

public:
  Q_INVOKABLE explicit NXWindow(QWidget *parent = nullptr);
  ~NXWindow() override;

  void moveToCenter() noexcept;

  void setCustomWidget(NXAppBarType::CustomArea customArea,
                       QWidget *customWidget,
                       QObject *hitTestObject             = nullptr,
                       const QString& hitTestFunctionName = {}) noexcept;
  QWidget *getCustomWidget(NXAppBarType::CustomArea customArea) const noexcept;

  void setCentralCustomWidget(QWidget *customWidget) noexcept;
  QWidget *getCentralCustomWidget() const noexcept;

  void setCustomMenu(QMenu *customMenu) noexcept;
  QMenu *getCustomMenu() const noexcept;

  void setIsLeftButtonPressedToggleNavigation(bool isPressed) noexcept;
  void setNavigationNodeDragAndDropEnable(bool isEnable) noexcept;
  void setUserInfoCardVisible(bool isVisible) noexcept;
  void setUserInfoCardPixmap(const QPixmap& pix) noexcept;
  void setUserInfoCardTitle(const QString& title) noexcept;
  void setUserInfoCardSubTitle(const QString& subTitle) noexcept;

  NXNavigationType::NodeOperateError setNavigationNodeTitle(const QString& nodeTitle, const QString& nodeKey) noexcept;
  QString getNavigationNodeTitle(const QString& nodeKey) const noexcept;

  QString addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome = NXIconType::None) const noexcept;
  NXNodeOperateResult addExpanderNode(const QString& expanderTitle,
                                      const QString& targetExpanderKey,
                                      NXIconType::IconName awesome = NXIconType::None) const noexcept;

  QString addCategoryNode(const QString& categoryTitle) noexcept;
  NXNodeOperateResult addCategoryNode(const QString& categoryTitle, const QString& targetExpanderKey) noexcept;

  NXNodeOperateResult
  addPageNode(const QString& pageTitle, QWidget *page, NXIconType::IconName awesome = NXIconType::None) noexcept;
  NXNodeOperateResult addPageNode(const QString& pageTitle,
                                  QWidget *page,
                                  const QString& targetExpanderKey,
                                  NXIconType::IconName awesome = NXIconType::None) noexcept;
  NXNodeOperateResult addPageNode(const QString& pageTitle,
                                  QWidget *page,
                                  int keyPoints                = 0,
                                  NXIconType::IconName awesome = NXIconType::None) noexcept;
  NXNodeOperateResult addPageNode(const QString& pageTitle,
                                  QWidget *page,
                                  const QString& targetExpanderKey,
                                  int keyPoints                = 0,
                                  NXIconType::IconName awesome = NXIconType::None) noexcept;

  NXNodeOperateResult addFooterNode(const QString& footerTitle,
                                    int keyPoints                = 0,
                                    NXIconType::IconName awesome = NXIconType::None) const noexcept;
  NXNodeOperateResult addFooterNode(const QString& footerTitle,
                                    QWidget *page,
                                    int keyPoints                = 0,
                                    NXIconType::IconName awesome = NXIconType::None) noexcept;

  void addCentralWidget(QWidget *centralWidget) noexcept;
  QWidget *getCentralWidget(int index) const noexcept;

  QString getNavigationRootKey() const noexcept;
  bool getNavigationNodeIsExpanded(const QString& expanderKey) const noexcept;
  void expandNavigationNode(const QString& expanderKey) noexcept;
  void collapseNavigationNode(const QString& expanderKey) noexcept;
  // 内部已经执行page的deleteLater()
  void removeNavigationNode(const QString& nodeKey) const noexcept;
  int getPageOpenInNewWindowCount(const QString& nodeKey) const noexcept;
  void backtrackNavigationNode(const QString& nodeKey) noexcept;

  void setNodeKeyPoints(const QString& nodeKey, int keyPoints) noexcept;
  int getNodeKeyPoints(const QString& nodeKey) const noexcept;

  void navigation(const QString& pageKey) noexcept;
  int getCurrentNavigationIndex() const noexcept;
  QString getCurrentNavigationPageKey() const noexcept;
  QList<NXSuggestBox::SuggestData> getNavigationSuggestDataList() const noexcept;

  void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true) noexcept;
  void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags) noexcept;
  NXAppBarType::ButtonFlags getWindowButtonFlags() const noexcept;

  void setWindowMoviePath(NXThemeType::ThemeMode themeMode, const QString& moviePath) noexcept;
  QString getWindowMoviePath(NXThemeType::ThemeMode themeMode) const noexcept;

  void setWindowPixmap(NXThemeType::ThemeMode themeMode, const QPixmap& pixmap) noexcept;
  QPixmap getWindowPixmap(NXThemeType::ThemeMode themeMode) const noexcept;

  void setWindowMovieRate(qreal rate) noexcept;
  qreal getWindowMovieRate() const noexcept;

  void closeWindow() noexcept;
  Q_SLOT NXNavigationType::NodeOperateError navigationPageNodeSwitch(const QString& targetPageKey) noexcept;
  void setNavigationPageOpenPolicy(std::function<void(const QString& /*nodeKey*/)>&& openNavigationPageFunc) noexcept;

Q_SIGNALS:
  void userInfoCardClicked();
  void closeButtonClicked();
  void navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType,
                             const QString& clickedNodeKey,
                             QWidget *clickedWidget);
  void navigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType,
                             const QString& showNavWidgetNodeKey,
                             QWidget *showNavWidget);
  void customWidgetChanged();
  void centralCustomWidgetChanged();
  void customMenuChanged();
  void pageOpenInNewWindow(const QString& nodeKey);

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;
  QMenu *createPopupMenu() override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXWINDOW_H
