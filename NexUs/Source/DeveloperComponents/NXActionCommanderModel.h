#ifndef NXACTIONCOMMANDERMODEL_H
#define NXACTIONCOMMANDERMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QObject>

#include "NXActionCommander.h"

class NXActionCommanderNode;

class NXActionCommanderModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit NXActionCommanderModel(QObject *parent = nullptr);
  ~NXActionCommanderModel() override;

  QModelIndex parent(const QModelIndex &child) const override;
  QModelIndex index(int row, int column, const QModelIndex &parent) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;

  void addCommand(NXActionCommand *command);
  void removeCommand(NXActionCommand *command);
  void updateCommand(NXActionCommand *command, bool isRedo);
  void clearCommand();

  const QModelIndex &getCommandModelIndex(NXActionCommand *command);

private:
  NXActionCommanderNode *_rootNode { nullptr };
  QMap<NXActionCommand *, NXActionCommanderNode *> _commanderNodeMap;
  void _updateNodeModelIndex(const QModelIndex &parentIndex = {});
};

#endif //NXACTIONCOMMANDERMODEL_H
