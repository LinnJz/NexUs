#include "NXNavigationNode.h"

#include <QUuid>

NXNavigationNode::NXNavigationNode(const QString& nodeTitle, NXNavigationNode *parent)
    : QObject(parent)
    , _pNodeKey(QString {})
{
  _pDepth           = 0;
  _pKeyPoints       = 0;
  _pNodeKey         = QUuid::createUuid().toString().remove(QStringLiteral("{")).remove(QStringLiteral("}")).remove(QStringLiteral("-"));
  _pNodeTitle       = nodeTitle;
  _pIsExpanded      = false;
  _pIsRootNode      = false;
  _pIsFooterNode    = false;
  _pIsHasFooterPage = false;
  _pParentNode      = parent;
  _pIsExpanderNode  = false;
  _pIsVisible       = false;
  _pIsCategoryNode  = false;
  _pAwesome         = NXIconType::None;
}

NXNavigationNode::~NXNavigationNode()
{
  // qDebug() << std::format("Title: {} Key: {}\n", _pNodeTitle.toStdString(), _pNodeKey.toStdString());
  // qDebug() << _pNodeTitle + " Prepare to delete " __FUNCTION__ "\n";
  // qDeleteAll(_pChildrenNodes);
  // qDebug() << _pNodeTitle + " Already delete " __FUNCTION__ "\n";
}

QString NXNavigationNode::getNodeKey() const noexcept { return _pNodeKey; }

void NXNavigationNode::setIsExpanded(bool isExpanded) noexcept
{
  _pIsExpanded = isExpanded;
  setChildVisible(isExpanded);
}

bool NXNavigationNode::getIsExpanded() const noexcept { return _pIsExpanded; }

void NXNavigationNode::setChildVisible(bool isVisible) noexcept
{
  if (isVisible)
  {
    for (auto node : _pChildrenNodes)
    {
      node->setIsVisible(isVisible);
      if (node->getIsExpanderNode() && !node->getIsExpanded()) { continue; }
      node->setChildVisible(isVisible);
    }
  }
  else
  {
    for (auto node : _pChildrenNodes)
    {
      node->setChildVisible(isVisible);
      node->setIsVisible(isVisible);
    }
  }
}

bool NXNavigationNode::getIsHasChild() const noexcept
{
  if (_pChildrenNodes.count() > 0) { return true; }
  return false;
}

bool NXNavigationNode::getIsHasPageChild() const noexcept
{
  if (_pChildrenNodes.count() == 0) { return false; }
  for (auto childNode : _pChildrenNodes)
  {
    if (!childNode->getIsExpanderNode()) { return true; }
    if (childNode->getIsHasPageChild()) { return true; }
  }
  return false;
}

void NXNavigationNode::appendChildNode(NXNavigationNode *childNode) noexcept
{
  if (_pIsExpanderNode) // 根节点也是ExpanderNode
  {
    _pChildrenNodes.append(childNode);
  }
}

void NXNavigationNode::removeChildNode(NXNavigationNode *childNode) noexcept
{
  if (_pIsExpanderNode) { _pChildrenNodes.removeOne(childNode); }
}

void NXNavigationNode::insertChildNode(int row, NXNavigationNode *childNode) noexcept
{
  if (row < 0 || row > _pChildrenNodes.count()) return;
  _pChildrenNodes.insert(row, childNode);
  childNode->setParentNode(this);
  childNode->setParent(this);
}

bool NXNavigationNode::getIsChildHasKeyPoints() const noexcept
{
  for (auto childNnode : _pChildrenNodes)
  {
    if (childNnode->getKeyPoints()) { return true; }
    if (childNnode->getIsChildHasKeyPoints()) { return true; }
  }
  return false;
}

NXNavigationNode *NXNavigationNode::getOriginalNode() noexcept
{
  if (this->getParentNode()->getIsRootNode()) { return this; }
  else
  {
    NXNavigationNode *originalNode = this->getParentNode();
    while (!originalNode->getIsRootNode() && !originalNode->getParentNode()->getIsRootNode())
    {
      originalNode = originalNode->getParentNode();
    }
    return originalNode;
  }
}

bool NXNavigationNode::getIsChildNode(NXNavigationNode *node) const noexcept
{
  if (_pChildrenNodes.count() > 0)
  {
    if (_pChildrenNodes.contains(node)) { return true; }
    for (auto childNnode : _pChildrenNodes)
    {
      if (childNnode->getIsChildNode(node)) { return true; }
    }
  }
  return false;
}

int NXNavigationNode::getRow() const noexcept
{
  if (_pParentNode) { return _pParentNode->getChildrenNodes().indexOf(const_cast<NXNavigationNode *>(this)); }
  return 0;
}

QList<NXNavigationNode *> NXNavigationNode::getExceptCategoryNodes() const noexcept
{
  QList<NXNavigationNode *> exceptCategoryNodeList;
  for (auto node : _pChildrenNodes)
  {
    if (!node->getIsCategoryNode()) { exceptCategoryNodeList.append(node); }
  }
  return exceptCategoryNodeList;
}

void NXNavigationNode::swap(NXNavigationNode *other)
{
  if (this == other || other == nullptr) return;
  std::swap(this->_pAwesome, other->_pAwesome);
  std::swap(this->_pKeyPoints, other->_pKeyPoints);
  std::swap(this->_pNodeTitle, other->_pNodeTitle);
  std::swap(this->_pNodeKey, other->_pNodeKey);
}
