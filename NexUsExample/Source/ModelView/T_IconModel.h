#ifndef T_ICONMODEL_H
#define T_ICONMODEL_H

#include <QAbstractListModel>
#include <QMetaEnum>

#include "NXProperty.h"

class T_IconModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY_CREATE(bool, IsSearchMode)

public:
  explicit T_IconModel(QObject *parent = nullptr);
  ~T_IconModel();
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  void setSearchKeyList(const QStringList &list);
  QStringList getSearchKeyList();

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QString getIconNameFromModelIndex(const QModelIndex &index) const;

private:
  int _rowCount;
  QStringList _searchKeyList;
  QMetaEnum _metaEnum;
};

#endif // T_ICONMODEL_H
