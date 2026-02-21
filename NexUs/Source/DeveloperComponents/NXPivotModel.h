#ifndef NXPIVOTMODEL_H
#define NXPIVOTMODEL_H

#include <QAbstractListModel>

class NXPivotModel : public QAbstractListModel
{
  Q_OBJECT
public:
  explicit NXPivotModel(QObject *parent = nullptr);
  ~NXPivotModel();

  void appendPivot(const QString& pivot);
  void removePivot(const QString& pivot);

  int getPivotListCount() const;

protected:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;

private:
  QStringList _pivotList;
};

#endif // NXPIVOTMODEL_H
