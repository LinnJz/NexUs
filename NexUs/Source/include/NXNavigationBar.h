#ifndef NXNAVIGATIONBAR_H
#define NXNAVIGATIONBAR_H

#include <QWidget>

#include "NXDef.h"
#include "NXSuggestBox.h"
class NXNavigationBarPrivate;

class NX_EXPORT NXNavigationBar : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXNavigationBar)
  Q_PROPERTY_CREATE_H(bool, IsTransparent)
  Q_PROPERTY_CREATE_H(bool, IsAllowPageOpenInNewWindow)
  Q_PROPERTY_CREATE_H(int, NavigationBarWidth)

public:
  explicit NXNavigationBar(QWidget *parent = nullptr);
  ~NXNavigationBar() override;
  void setUserInfoCardVisible(bool isVisible) noexcept;
  void setUserInfoCardPixmap(const QPixmap& pix) noexcept;
  void setUserInfoCardTitle(const QString& title) noexcept;
  void setUserInfoCardSubTitle(const QString& subTitle) noexcept;
  void setNavigationPageOpenPolicy(std::function<void(const QString& /*nodeKey*/)>&& openNavigationPageFunc) noexcept;
  void setIsLeftButtonPressedToggleNavigation(bool isPressed) noexcept;
  void setNavigationNodeDragAndDropEnable(bool isEnable) noexcept;
  void setToolTipOffset(int offsetX, int offsetY) noexcept;

  QString addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome = NXIconType::None) noexcept;
  NXNodeOperateResult addExpanderNode(const QString& expanderTitle,
                                      const QString& targetExpanderKey,
                                      NXIconType::IconName awesome = NXIconType::None) noexcept;

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
                                    NXIconType::IconName awesome = NXIconType::None) noexcept;
  NXNodeOperateResult addFooterNode(const QString& footerTitle,
                                    QWidget *page,
                                    int keyPoints                = 0,
                                    NXIconType::IconName awesome = NXIconType::None) noexcept;

  QString getNavigationRootKey() const noexcept;
  bool getNavigationNodeIsExpanded(const QString& expanderKey) const noexcept;
  void expandNavigationNode(const QString& expanderKey) noexcept;
  void collapseNavigationNode(const QString& expanderKey) noexcept;
  void removeNavigationNode(const QString& nodeKey) noexcept;

  void setNodeKeyPoints(const QString& nodeKey, int keyPoints) noexcept;
  int getNodeKeyPoints(const QString& nodeKey) const noexcept;

  NXNavigationType::NodeOperateError setNavigationNodeTitle(const QString& nodeTitle, const QString& nodeKey) noexcept;
  QString getNavigationNodeTitle(const QString& nodeKey) const noexcept;

  void setDisplayMode(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true) noexcept;
  NXNavigationType::NavigationDisplayMode getDisplayMode() const noexcept;

  void navigation(const QString& pageKey, bool isLogClicked = true, bool isRouteBack = false) noexcept;
  int getPageOpenInNewWindowCount(const QString& nodeKey) const noexcept;
  QList<NXSuggestBox::SuggestData> getSuggestDataList() const noexcept;

  Q_SLOT NXNavigationType::NodeOperateError navigationPageNodeSwitch(const QString& targetPageNodeKey) noexcept;
Q_SIGNALS:
  void pageOpenInNewWindow(const QString& nodeKey);
  void userInfoCardClicked();
  void
  navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, const QString& clickedNodeKey, bool isRouteBack);
  void navigationNodeAdded(NXNavigationType::NavigationNodeType nodeType, const QString& addedNodeKey, QWidget *page);
  void navigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, const QString& removedNodeKey);
  void displayModeChanged(NXNavigationType::NavigationDisplayMode displayMode);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXNAVIGATIONBAR_H
