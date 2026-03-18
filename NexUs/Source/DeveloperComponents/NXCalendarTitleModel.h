#ifndef NXCALENDARTITLEMODEL_H
#define NXCALENDARTITLEMODEL_H

#include <QAbstractListModel>

class NXCalendarTitleModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit NXCalendarTitleModel(QObject *parent = nullptr);
  ~NXCalendarTitleModel();

protected:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;
};

#endif // NXCALENDARTITLEMODEL_H
