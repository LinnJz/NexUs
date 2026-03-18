#ifndef NXSUGGESTMODEL_H
#define NXSUGGESTMODEL_H

#include <QAbstractListModel>

#include "NXDef.h"
class NXSuggestion;

class NXSuggestModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit NXSuggestModel(QObject *parent = nullptr);
  ~NXSuggestModel();
  int rowCount(const QModelIndex& parent) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  void setSearchSuggestion(const QList<NXSuggestion *>& suggestionVector) noexcept;
  void clearSearchNode() noexcept;
  NXSuggestion *getSearchSuggestion(int row) const noexcept;

private:
  QList<NXSuggestion *> _suggestionVector; // 符合搜索的节点
};

#endif // NXSUGGESTMODEL_H
