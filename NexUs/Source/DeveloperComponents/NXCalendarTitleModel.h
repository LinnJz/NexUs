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
  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex& index, int role) const override;
};

#endif // NXCALENDARTITLEMODEL_H
