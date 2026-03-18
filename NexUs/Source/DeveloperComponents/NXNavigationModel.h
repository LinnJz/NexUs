#ifndef NXNAVIGATIONMODEL_H
#define NXNAVIGATIONMODEL_H

#include <QAbstractItemModel>
#include <QObject>

#include "NXDef.h"
class NXNavigationNode;

class NXNavigationModel : public QAbstractItemModel
{
  Q_OBJECT
  Q_PRIVATE_CREATE(NXNavigationNode *, SelectedNode)
  Q_PRIVATE_CREATE(NXNavigationNode *, SelectedExpandedNode)

public:
  explicit NXNavigationModel(QObject *parent = nullptr);
  ~NXNavigationModel() override;
  QModelIndex parent(const QModelIndex& child) const override;
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  void setIsMaximalMode(bool isMaximal) noexcept;
  bool getIsMaximalMode() const noexcept;

  QString addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome) noexcept;
  NXNodeOperateResult addExpanderNode(const QString& expanderTitle,
                                      const QString& targetExpanderKey,
                                      NXIconType::IconName awesome) noexcept;

  QString addCategoryNode(const QString& categoryTitle) noexcept;
  NXNodeOperateResult addCategoryNode(const QString& categoryTitle, const QString& targetExpanderKey) noexcept;

  QString addPageNode(const QString& pageTitle, NXIconType::IconName awesome) noexcept;
  NXNodeOperateResult
  addPageNode(const QString& pageTitle, const QString& targetExpanderKey, NXIconType::IconName awesome) noexcept;
  QString addPageNode(const QString& pageTitle, int keyPoints, NXIconType::IconName awesome) noexcept;
  NXNodeOperateResult addPageNode(const QString& pageTitle,
                                  const QString& targetExpanderKey,
                                  int keyPoints,
                                  NXIconType::IconName awesome) noexcept;

  QStringList removeNavigationNode(const QString& nodeKey) noexcept;

  const NXNavigationNode *getRootNode() const noexcept;
  NXNavigationNode *getNavigationNode(const QString& nodeKey) const noexcept;
  QList<NXNavigationNode *> getRootExpanderNodes() const noexcept;
  QList<NXNavigationNode *> getRootExpandedNodes() const noexcept;
  QList<NXNavigationNode *> getRootCategoryNodes() const noexcept;

  Qt::DropActions supportedDropActions() const override;
  Qt::DropActions supportedDragActions() const override;
  QStringList mimeTypes() const override;
  QMimeData *mimeData(const QModelIndexList& indexes) const override;
  bool canDropMimeData(
      const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
  bool
  dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

  bool swapTwoNodes(const QString& nodeKey1, const QString& nodeKey2) noexcept;

Q_SIGNALS:
  void mineDataDropped(const QMimeData *data, const QModelIndex& draggedIndex, const QModelIndex& targetIndex);

private:
  QMap<QString, NXNavigationNode *> _nodesMap;
  NXNavigationNode *_rootNode { nullptr };
  bool _isMaximalMode { true };
};

#endif // NXNAVIGATIONMODEL_H
