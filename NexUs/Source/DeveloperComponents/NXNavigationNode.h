#ifndef NXNAVIGATIONNODE_H
#define NXNAVIGATIONNODE_H

#include <QModelIndex>
#include <QObject>

#include "NXDef.h"

class NXNavigationNode : public QObject
{
  Q_OBJECT
  Q_PROPERTY_CREATE(QList<NXNavigationNode *>, ChildrenNodes)
  Q_PRIVATE_CREATE(NXNavigationNode *, ParentNode)
  Q_PROPERTY_CREATE(NXIconType::IconName, Awesome)
  Q_PROPERTY_CREATE(QModelIndex, ModelIndex)
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
  Q_PROPERTY_CREATE_EX(const QString&, QString, NodeTitle)
public:
  explicit NXNavigationNode(const QString& nodeTitle, NXNavigationNode *parent = nullptr);
  ~NXNavigationNode() override;

  QString getNodeKey() const;

  void setIsExpanded(bool isExpanded);
  bool getIsExpanded() const;

  void setChildVisible(bool isVisible);
  bool getIsHasChild() const;
  bool getIsHasPageChild() const;

  void appendChildNode(NXNavigationNode *childNode);
  void removeChildNode(NXNavigationNode *childNode);
  void insertChildNode(int row, NXNavigationNode *childNode);

  bool getIsChildHasKeyPoints() const;

  NXNavigationNode *getOriginalNode();
  bool getIsChildNode(NXNavigationNode *node) const;

  int getRow() const;

  QList<NXNavigationNode *> getExceptCategoryNodes();

  void swapVisual(NXNavigationNode *other);
};

#endif // NXNAVIGATIONNODE_H
