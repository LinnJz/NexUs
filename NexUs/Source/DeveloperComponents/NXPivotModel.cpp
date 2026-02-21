#include "NXPivotModel.h"

NXPivotModel::NXPivotModel(QObject *parent)
    : QAbstractListModel { parent }
{
}

NXPivotModel::~NXPivotModel() { }

void NXPivotModel::appendPivot(const QString& pivot)
{
  if (!pivot.isEmpty())
  {
    beginInsertRows(QModelIndex(), _pivotList.count(), _pivotList.count());
    _pivotList.append(pivot);
    endInsertRows();
    return;
  }
}

void NXPivotModel::removePivot(const QString& pivot)
{
  if (_pivotList.contains(pivot))
  {
    int index = _pivotList.lastIndexOf(pivot);
    beginRemoveRows(QModelIndex(), index, index);
    _pivotList.removeAt(index);
    endRemoveRows();
  }
}

int NXPivotModel::getPivotListCount() const { return _pivotList.count(); }

int NXPivotModel::rowCount(const QModelIndex& parent) const { return _pivotList.count(); }

QVariant NXPivotModel::data(const QModelIndex& index, int role) const
{
  if (role == Qt::DisplayRole) { return _pivotList[index.row()]; }
  return QVariant();
}
