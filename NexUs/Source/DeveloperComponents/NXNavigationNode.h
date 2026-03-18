#ifndef NXNAVIGATIONNODE_H
#define NXNAVIGATIONNODE_H

#include <QModelIndex>
#include <QObject>

#include "NXDef.h"

class NXNavigationNode : public QObject
{
  Q_OBJECT
  Q_PROPERTY_CREATE_2(const QList<NXNavigationNode *>&, auto, ChildrenNodes)
  Q_PROPERTY_CREATE_2(const QModelIndex&, QModelIndex, ModelIndex)
  Q_PRIVATE_CREATE(NXNavigationNode *, ParentNode)
  Q_PROPERTY_CREATE(NXIconType::IconName, Awesome)
  Q_PROPERTY_CREATE(int, KeyPoints)
  Q_PROPERTY_CREATE(int, Depth)
  Q_PROPERTY_CREATE(bool, IsRootNode)
  Q_PROPERTY_CREATE(bool, IsFooterNode)
  Q_PROPERTY_CREATE(bool, IsHasFooterPage)
  Q_PROPERTY_CREATE(bool, IsExpanderNode)
  Q_PROPERTY_CREATE(bool, IsVisible)
  Q_PROPERTY_CREATE_D(bool, IsExpanded)
  Q_PRIVATE_CREATE(bool, IsCategoryNode)
  Q_PROPERTY_CREATE_D(QString, NodeKey)
  Q_PROPERTY_CREATE_2(const QString&, QString, NodeTitle)

public:
  explicit NXNavigationNode(const QString& nodeTitle, NXNavigationNode *parent = nullptr);
  ~NXNavigationNode() override;

  QString getNodeKey() const noexcept;

  void setIsExpanded(bool isExpanded) noexcept;
  bool getIsExpanded() const noexcept;

  void setChildVisible(bool isVisible) noexcept;
  bool getIsHasChild() const noexcept;
  bool getIsHasPageChild() const noexcept;

  void appendChildNode(NXNavigationNode *childNode) noexcept;
  void removeChildNode(NXNavigationNode *childNode) noexcept;
  void insertChildNode(int row, NXNavigationNode *childNode) noexcept;

  bool getIsChildHasKeyPoints() const noexcept;

  NXNavigationNode *getOriginalNode() noexcept;
  bool getIsChildNode(NXNavigationNode *node) const noexcept;

  int getRow() const noexcept;

  QList<NXNavigationNode *> getExceptCategoryNodes() const noexcept;

  void swap(NXNavigationNode *other);
};

#endif // NXNAVIGATIONNODE_H
