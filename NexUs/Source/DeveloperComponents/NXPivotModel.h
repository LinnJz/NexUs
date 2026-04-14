#ifndef NXPIVOTMODEL_H
#define NXPIVOTMODEL_H

#include <QAbstractListModel>
#include <QPixmap>

#include "NXDef.h"

class NXPivotModel : public QAbstractListModel
{
  Q_OBJECT

public:
  struct PivotItem
  {
    QString text;
    QPixmap icon;
  };

  explicit NXPivotModel(QObject *parent = nullptr);
  ~NXPivotModel() override;

  void appendPivot(const QPixmap &pixmap) noexcept;
  void appendPivot(const QString &text, const QPixmap &pixmap = {}) noexcept;

  void insertPivot(int index, const QPixmap &pixmap) noexcept;
  void insertPivot(int index, const QString &text, const QPixmap &pixmap = {}) noexcept;

  void removePivot(int index) noexcept;

  void setPivot(int index, const QPixmap &pixmap) noexcept;
  void setPivot(int index, const QString &text, const QPixmap &pixmap = {}) noexcept;

  int getPivotListCount() const noexcept;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;

private:
  QList<PivotItem> _pivotList;
};

Q_DECLARE_METATYPE(NXPivotModel::PivotItem);

#endif // NXPIVOTMODEL_H
