#ifndef NXBREADCRUMBBARMODEL_H
#define NXBREADCRUMBBARMODEL_H

#include <QAbstractListModel>

class NXBreadcrumbBarModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit NXBreadcrumbBarModel(QObject *parent = nullptr);
  ~NXBreadcrumbBarModel();

  void appendBreadcrumb(const QString& breadcrumb) noexcept;
  void removeBreadcrumb(const QString& breadcrumb) noexcept;
  void removeBreadcrumb(int index) noexcept;

  void setBreadcrumbList(const QStringList& breadcrumbList) noexcept;

  int getBreadcrumbListCount() const noexcept;
  QStringList getBreadcrumbList() const noexcept;

protected:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;

private:
  QStringList _breadcrumbList;
};

#endif // NXBREADCRUMBBARMODEL_H
