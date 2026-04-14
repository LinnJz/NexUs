#include "NXPivotModel.h"

#include <QFontMetrics>
#include <QSize>
#include <QtGlobal>

NXPivotModel::NXPivotModel(QObject *parent)
    : QAbstractListModel { parent }
{
}

NXPivotModel::~NXPivotModel()
{
}

void
NXPivotModel::appendPivot(const QPixmap &pixmap) noexcept
{
  appendPivot(QString {}, pixmap);
}

void
NXPivotModel::appendPivot(const QString &text, const QPixmap &pixmap) noexcept
{
  insertPivot(_pivotList.count(), text, pixmap);
}

void
NXPivotModel::insertPivot(int index, const QPixmap &pixmap) noexcept
{
  insertPivot(index, QString {}, pixmap);
}

void
NXPivotModel::insertPivot(int index, const QString &text, const QPixmap &pixmap) noexcept
{
  beginInsertRows(QModelIndex(), index, index);
  _pivotList.insert(index, { text, pixmap });
  endInsertRows();
}

void
NXPivotModel::removePivot(int index) noexcept
{
  beginRemoveRows(QModelIndex(), index, index);
  _pivotList.removeAt(index);
  endRemoveRows();
}

void
NXPivotModel::setPivot(int index, const QPixmap &pixmap) noexcept
{
  _pivotList[index].icon = pixmap;
  Q_EMIT dataChanged(this->index(index, 0), this->index(index, 0),
                     { Qt::UserRole, Qt::DisplayRole, Qt::DecorationRole, Qt::SizeHintRole });
}

void
NXPivotModel::setPivot(int index, const QString &text, const QPixmap &pixmap) noexcept
{
  _pivotList[index].text = text;
  _pivotList[index].icon = pixmap;
  Q_EMIT dataChanged(this->index(index, 0), this->index(index, 0),
                     { Qt::UserRole, Qt::DisplayRole, Qt::DecorationRole, Qt::SizeHintRole });
}

int
NXPivotModel::getPivotListCount() const noexcept
{
  return _pivotList.count();
}

int
NXPivotModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return 0;
  }
  return _pivotList.count();
}

QVariant
NXPivotModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= _pivotList.count())
  {
    return QVariant {};
  }

  const PivotItem &item = _pivotList[index.row()];
  if (role == Qt::DisplayRole)
  {
    return item.text;
  }
  if (role == Qt::UserRole)
  {
    return QVariant::fromValue(item);
  }
  return QVariant {};
}
