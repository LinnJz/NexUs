#include "NXNavigationModel.h"

#include <QIODevice>
#include <QIcon>
#include <QJsonObject>
#include <QMimeData>
#include "NXNavigationNode.h"

static void RecursiveUpdateDepth_(NXNavigationNode *node, int baseDepth)
{
  node->setDepth(baseDepth + 1);
  for (auto child : node->getChildrenNodes()) { RecursiveUpdateDepth_(child, baseDepth + 1); }
}

NXNavigationModel::NXNavigationModel(QObject *parent)
    : QAbstractItemModel { parent }
{
  _rootNode = new NXNavigationNode("root");
  _rootNode->setIsRootNode(true);
  _rootNode->setIsExpanderNode(true);
  _rootNode->setIsExpanded(true);
  _nodesMap.insert(_rootNode->getNodeKey(), _rootNode);
  _pSelectedNode         = nullptr;
  _pSelectedExpandedNode = nullptr;
}

NXNavigationModel::~NXNavigationModel() { delete _rootNode; }

QModelIndex NXNavigationModel::parent(const QModelIndex& child) const
{
  if (!child.isValid()) { return QModelIndex(); }
  NXNavigationNode *childNode  = static_cast<NXNavigationNode *>(child.internalPointer());
  NXNavigationNode *parentNode = childNode->getParentNode();
  if (parentNode == _rootNode) { return QModelIndex(); }
  if (parentNode == nullptr) { return QModelIndex(); }
  return createIndex(parentNode->getRow(), 0, parentNode);
}

QModelIndex NXNavigationModel::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent)) { return QModelIndex(); }
  NXNavigationNode *parentNode;
  if (!parent.isValid()) { parentNode = _rootNode; }
  else
  {
    parentNode = static_cast<NXNavigationNode *>(parent.internalPointer());
  }
  NXNavigationNode *childNode = nullptr;
  if (parentNode->getChildrenNodes().count() > row)
  {
    if (parentNode == _rootNode && !_isMaximalMode) { childNode = parentNode->getExceptCategoryNodes()[row]; }
    else
    {
      childNode = parentNode->getChildrenNodes()[row];
    }
  }
  if (childNode) { return createIndex(row, column, childNode); }
  return QModelIndex();
}

int NXNavigationModel::rowCount(const QModelIndex& parent) const
{
  NXNavigationNode *parentNode;
  if (parent.column() > 0) { return 0; }
  if (!parent.isValid()) { parentNode = _rootNode; }
  else
  {
    parentNode = static_cast<NXNavigationNode *>(parent.internalPointer());
  }
  if (parentNode == _rootNode && !_isMaximalMode) { return _rootNode->getExceptCategoryNodes().count(); }
  return parentNode->getChildrenNodes().count();
};

int NXNavigationModel::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return 1;
}

QVariant NXNavigationModel::data(const QModelIndex& index, int role) const
{
  Q_UNUSED(index)
  Q_UNUSED(role)
  return QVariant();
}

Qt::ItemFlags NXNavigationModel::flags(const QModelIndex& index) const
{
  if (!index.isValid()) { return Qt::NoItemFlags; }
  auto flags = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
  if (index.column() != 0) { flags |= Qt::ItemIsEditable; }
  return flags;
}

void NXNavigationModel::setIsMaximalMode(bool isMaximal) { _isMaximalMode = isMaximal; }

bool NXNavigationModel::getIsMaximalMode() { return _isMaximalMode; }

QString NXNavigationModel::addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome)
{
  NXNavigationNode *node = new NXNavigationNode(expanderTitle, _rootNode);
  node->setDepth(1);
  node->setIsVisible(true);
  node->setIsExpanderNode(true);
  node->setAwesome(awesome);
  beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
  _rootNode->appendChildNode(node);
  _nodesMap.insert(node->getNodeKey(), node);
  endInsertRows();
  return node->getNodeKey();
}

NXNodeOperateResult NXNavigationModel::addExpanderNode(const QString& expanderTitle,
                                                       const QString& targetExpanderKey,
                                                       NXIconType::IconName awesome)
{
  if (!_nodesMap.contains(targetExpanderKey))
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeInvalid };
  }
  NXNavigationNode *parentNode = _nodesMap.value(targetExpanderKey);
  if (!parentNode->getIsExpanderNode())
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeTypeError };
  }
  if (parentNode->getDepth() > 10)
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeDepthLimit };
  }
  NXNavigationNode *node = new NXNavigationNode(expanderTitle, parentNode);
  node->setDepth(parentNode->getDepth() + 1);
  node->setIsExpanderNode(true);
  node->setAwesome(awesome);
  if (parentNode->getIsVisible() && parentNode->getIsExpanded()) { node->setIsVisible(true); }
  beginInsertRows(
      parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
  parentNode->appendChildNode(node);
  _nodesMap.insert(node->getNodeKey(), node);
  endInsertRows();
  return node->getNodeKey();
}

QString NXNavigationModel::addPageNode(const QString& pageTitle, NXIconType::IconName awesome)
{
  NXNavigationNode *node = new NXNavigationNode(pageTitle, _rootNode);
  node->setAwesome(awesome);
  node->setDepth(1);
  node->setIsVisible(true);
  beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
  _rootNode->appendChildNode(node);
  _nodesMap.insert(node->getNodeKey(), node);
  endInsertRows();
  if (!_pSelectedNode) { _pSelectedNode = node; }
  return node->getNodeKey();
}

NXNodeOperateResult
NXNavigationModel::addPageNode(const QString& pageTitle, const QString& targetExpanderKey, NXIconType::IconName awesome)
{
  if (!_nodesMap.contains(targetExpanderKey))
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeInvalid };
  }
  NXNavigationNode *parentNode = _nodesMap.value(targetExpanderKey);
  if (!parentNode->getIsExpanderNode())
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeTypeError };
  }
  if (parentNode->getDepth() > 10)
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeDepthLimit };
  }
  NXNavigationNode *node = new NXNavigationNode(pageTitle, parentNode);
  node->setDepth(parentNode->getDepth() + 1);
  node->setAwesome(awesome);
  if (parentNode->getIsVisible() && parentNode->getIsExpanded()) { node->setIsVisible(true); }
  beginInsertRows(
      parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
  parentNode->appendChildNode(node);
  _nodesMap.insert(node->getNodeKey(), node);
  endInsertRows();
  if (!_pSelectedNode) { _pSelectedNode = node; }
  return node->getNodeKey();
}

QString NXNavigationModel::addPageNode(const QString& pageTitle, int keyPoints, NXIconType::IconName awesome)
{
  NXNavigationNode *node = new NXNavigationNode(pageTitle, _rootNode);
  node->setAwesome(awesome);
  node->setDepth(1);
  node->setIsVisible(true);
  node->setKeyPoints(keyPoints);
  beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
  _rootNode->appendChildNode(node);
  _nodesMap.insert(node->getNodeKey(), node);
  endInsertRows();
  if (!_pSelectedNode) { _pSelectedNode = node; }
  return node->getNodeKey();
}

NXNodeOperateResult NXNavigationModel::addPageNode(const QString& pageTitle,
                                                   const QString& targetExpanderKey,
                                                   int keyPoints,
                                                   NXIconType::IconName awesome)
{
  if (!_nodesMap.contains(targetExpanderKey))
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeInvalid };
  }
  NXNavigationNode *parentNode = _nodesMap.value(targetExpanderKey);
  if (!parentNode->getIsExpanderNode())
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeTypeError };
  }
  if (parentNode->getDepth() > 10)
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeDepthLimit };
  }
  NXNavigationNode *node = new NXNavigationNode(pageTitle, parentNode);
  node->setDepth(parentNode->getDepth() + 1);
  node->setAwesome(awesome);
  node->setKeyPoints(keyPoints);
  if (parentNode->getIsVisible() && parentNode->getIsExpanded()) { node->setIsVisible(true); }
  beginInsertRows(
      parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
  parentNode->appendChildNode(node);
  _nodesMap.insert(node->getNodeKey(), node);
  endInsertRows();
  if (!_pSelectedNode) { _pSelectedNode = node; }
  return node->getNodeKey();
}

QString NXNavigationModel::addCategoryNode(const QString& categoryTitle)
{
  NXNavigationNode *node = new NXNavigationNode(categoryTitle, _rootNode);
  node->setDepth(1);
  node->setIsVisible(true);
  node->setIsCategoryNode(true);
  beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
  _rootNode->appendChildNode(node);
  _nodesMap.insert(node->getNodeKey(), node);
  endInsertRows();
  return node->getNodeKey();
}

NXNodeOperateResult NXNavigationModel::addCategoryNode(const QString& categoryTitle, const QString& targetExpanderKey)
{
  if (!_nodesMap.contains(targetExpanderKey))
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeInvalid };
  }
  NXNavigationNode *parentNode = _nodesMap.value(targetExpanderKey);
  if (!parentNode->getIsExpanderNode())
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeTypeError };
  }
  if (parentNode->getDepth() > 10)
  {
    return NXUnexpected<QString> { NXNavigationType::NodeOperateError::TargetNodeDepthLimit };
  }
  NXNavigationNode *node = new NXNavigationNode(categoryTitle, parentNode);
  node->setDepth(parentNode->getDepth() + 1);
  node->setIsCategoryNode(true);
  if (parentNode->getIsVisible() && parentNode->getIsExpanded()) { node->setIsVisible(true); }
  beginInsertRows(
      parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
  parentNode->appendChildNode(node);
  _nodesMap.insert(node->getNodeKey(), node);
  endInsertRows();
  return node->getNodeKey();
}

QStringList NXNavigationModel::removeNavigationNode(const QString& nodeKey)
{
  QList<QString> removeKeyList;
  if (!_nodesMap.contains(nodeKey)) { return removeKeyList; }
  NXNavigationNode *node       = _nodesMap.value(nodeKey);
  NXNavigationNode *parentNode = node->getParentNode();
  if (node->getIsExpanderNode())
  {
    QList<NXNavigationNode *> childNodeList = node->getChildrenNodes();
    for (int i = 0; i < childNodeList.count(); i++)
    {
      NXNavigationNode *childNode       = childNodeList[i];
      QList<QString> childRemoveKeyList = removeNavigationNode(childNode->getNodeKey());
      removeKeyList.append(childRemoveKeyList);
    }
  }
  else
  {
    removeKeyList.append(node->getNodeKey());
  }
  beginRemoveRows(parentNode->getModelIndex(),
                  parentNode->getChildrenNodes().count() - 1,
                  parentNode->getChildrenNodes().count() - 1);
  parentNode->removeChildNode(node);
  _nodesMap.remove(node->getNodeKey());
  endRemoveRows();
  return removeKeyList;
}

const NXNavigationNode *NXNavigationModel::getRootNode() const { return _rootNode; }

NXNavigationNode *NXNavigationModel::getNavigationNode(const QString& nodeKey) const
{
  if (_nodesMap.contains(nodeKey)) { return _nodesMap.value(nodeKey); }
  return nullptr;
}

QList<NXNavigationNode *> NXNavigationModel::getRootExpanderNodes() const
{
  QList<NXNavigationNode *> expandedNodeList;
  for (auto node : _rootNode->getChildrenNodes())
  {
    if (node->getIsExpanderNode()) { expandedNodeList.append(node); }
  }
  return expandedNodeList;
}

QList<NXNavigationNode *> NXNavigationModel::getRootExpandedNodes() const
{
  QList<NXNavigationNode *> expandedNodeList;
  for (auto node : _rootNode->getChildrenNodes())
  {
    if (node->getIsExpanderNode() && node->getIsExpanded()) { expandedNodeList.append(node); }
  }
  return expandedNodeList;
}

QList<NXNavigationNode *> NXNavigationModel::getRootCategoryNodes() const
{
  QList<NXNavigationNode *> categoryNodeList;
  for (auto node : _rootNode->getChildrenNodes())
  {
    if (node->getIsCategoryNode()) { categoryNodeList.append(node); }
  }
  return categoryNodeList;
}

Qt::DropActions NXNavigationModel::supportedDropActions() const { return Qt::MoveAction; }

Qt::DropActions NXNavigationModel::supportedDragActions() const { return Qt::MoveAction; }

QStringList NXNavigationModel::mimeTypes() const { return QStringList() << "application/x-nxnavigation-node"; }

QMimeData *NXNavigationModel::mimeData(const QModelIndexList& indexes) const
{
  QMimeData *mimeData = new QMimeData();
  if (indexes.isEmpty()) return mimeData;

  QModelIndex index      = indexes.first();
  NXNavigationNode *node = static_cast<NXNavigationNode *>(index.internalPointer());
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);
  stream << node->getNodeKey() << node->getNodeTitle();
  mimeData->setData("application/x-nxnavigation-node", encodedData);
  return mimeData;
}

bool NXNavigationModel::canDropMimeData(
    const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
  if (!data->hasFormat("application/x-nxnavigation-node") || action != Qt::MoveAction || _nodesMap.size() <= 1)
    return false;

  return true;
}

bool NXNavigationModel::dropMimeData(
    const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
  enum DropIndicatorPosition
  {
    OnItem,
    AboveItem,
    BelowItem,
    OnViewport
  };

  QByteArray encodedData = data->data("application/x-nxnavigation-node");
  QDataStream stream(&encodedData, QIODevice::ReadOnly);
  QString nodeKey, nodeTitle;
  stream >> nodeKey >> nodeTitle;
  qDebug() << "nodeKey: " << nodeKey << "\n nodeTitle: " << nodeTitle;

  NXNavigationNode *draggedNode = _nodesMap.value(nodeKey);
  if (!draggedNode) return false;

  const QModelIndex& targetIndex = property("NXDropTargetIndex").value<QModelIndex>();
  if (!targetIndex.isValid()) return false;
  DropIndicatorPosition dropindicationPos = property("NXDropIndicatorPosition").value<DropIndicatorPosition>();

  NXNavigationNode *targetNode        = static_cast<NXNavigationNode *>(targetIndex.internalPointer());
  NXNavigationNode *draggedParentNode = draggedNode->getParentNode();
  NXNavigationNode *targetParentNode  = targetIndex.isValid() ? targetNode->getParentNode() : _rootNode;
  const bool isSameParent             = (draggedParentNode == targetParentNode);

  int totalRow   = targetParentNode->getChildrenNodes().count();
  int targetRow  = (row == -1) ? totalRow : row;
  int draggedRow = draggedNode->getRow();

  // 单个节点移动，上移差1，下移差2（+1）
  if (isSameParent)
  {
    // if (targetRow >= totalRow) targetRow; else
    if (draggedRow < targetRow)
    {
      if (draggedRow + 1 == targetRow)
        targetRow++; // 差2纠正
      else if (dropindicationPos == DropIndicatorPosition::BelowItem)
        targetRow++; // 差2纠正
                     // else if(_dropIndicatorPosition == NXNavigationModel::AboveItem) targetRow;
    }
    else if (draggedRow > targetRow)
    {
      if (draggedRow - 1 != targetRow)
        if (dropindicationPos == DropIndicatorPosition::BelowItem) targetRow++; // 下移一个位置
    }
  }
  else
  {
    if (dropindicationPos == DropIndicatorPosition::BelowItem) targetRow++; // 下移一个位置
  }

  beginMoveRows(draggedParentNode->getModelIndex(), draggedRow, draggedRow, parent, targetRow);
  if (isSameParent)
  {
    if (targetRow >= totalRow || draggedRow < targetRow) targetRow--; // 差2纠正，需恢复，确保insert位置正确
  }
  draggedParentNode->removeChildNode(draggedNode);
  targetParentNode->insertChildNode(targetRow, draggedNode);

  // C++23
  /*auto recursiveUpdateDepth =
      [](this auto&& self, NXNavigationNode* node, int baseDepth) -> void {
      node->setDepth(baseDepth + 1);
      for (auto child : node->getChildrenNodes()) {
          self(child, baseDepth + 1);
      }
      };
  recursiveUpdateDepth(draggedNode, targetParentNode->getDepth());*/
  RecursiveUpdateDepth_(draggedNode, targetParentNode->getDepth());
  endMoveRows();
  Q_EMIT mineDataDropped(data, draggedNode->getModelIndex(), targetIndex);
  return true;
}

bool NXNavigationModel::swapNodes(const QString& nodeKey1, const QString& nodeKey2)
{
  auto it = _nodesMap.begin();
  unsigned flag { 0 };
  decltype(it) nodeKeyIt1;
  decltype(it) nodeKeyIt2;
  for (; it != _nodesMap.end(); ++it)
  {
    if (it.key() == nodeKey1)
    {
      nodeKeyIt1 = it;
      ++flag;
      continue;
    }
    if (it.key() == nodeKey2)
    {
      nodeKeyIt2 = it;
      ++flag;
      continue;
    }
    if (flag == 2)
    {
      std::swap(nodeKeyIt1.value(), nodeKeyIt2.value());
      return true;
    }
  }
  return false;
}
