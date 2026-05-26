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
  ~NXNavigationModel();

  void setIsMaximalMode(bool isMaximal);
  bool getIsMaximalMode();

  QModelIndex parent(const QModelIndex &child) const override;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

  QString addExpanderNode(const QString &expanderTitle, NXIconType::IconName awesome);
  NXNodeResultExpected
  addExpanderNode(const QString &expanderTitle, const QString &targetExpanderKey, NXIconType::IconName awesome);

  QString addPageNode(const QString &pageTitle, NXIconType::IconName awesome);
  NXNodeResultExpected
  addPageNode(const QString &pageTitle, const QString &targetExpanderKey, NXIconType::IconName awesome);
  QString addPageNode(const QString &pageTitle, int keyPoints, NXIconType::IconName awesome);
  NXNodeResultExpected
  addPageNode(const QString &pageTitle, const QString &targetExpanderKey, int keyPoints, NXIconType::IconName awesome);

  QString addCategoryNode(const QString &categoryTitle);
  NXNodeResultExpected addCategoryNode(const QString &categoryTitle, const QString &targetExpanderKey);

  QStringList removeNavigationNode(const QString &nodeKey);

  const NXNavigationNode *getRootNode() const;
  NXNavigationNode *getNavigationNode(const QString &nodeKey) const;
  QList<NXNavigationNode *> getRootExpanderNodes() const;
  QList<NXNavigationNode *> getRootExpandedNodes() const;

  QList<NXNavigationNode *> getRootCategoryNodes() const;

private:
  bool _isMaximalMode { true };
  QMap<QString, NXNavigationNode *> _nodesMap;
  NXNavigationNode *_rootNode { nullptr };
};

#endif // NXNAVIGATIONMODEL_H
