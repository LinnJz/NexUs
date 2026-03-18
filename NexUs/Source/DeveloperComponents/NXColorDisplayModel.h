#ifndef NXCOLORDISPLAYMODEL_H
#define NXCOLORDISPLAYMODEL_H

#include <QAbstractListModel>
#include <QColor>

class NXColorDisplayModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit NXColorDisplayModel(QObject *parent = nullptr);
  ~NXColorDisplayModel();
  void appendDisplayColor(const QList<QColor>& colorList) noexcept;
  void appendDisplayColor(const QColor& color) noexcept;
  void removeDisplayColor(int index) noexcept;
  void replaceDisplayColor(const QColor& color, int index) noexcept;

  QList<QColor> getDisplayColorList() const noexcept;
  QColor getDisplayColor(int index) const noexcept;

protected:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;

private:
  QList<QColor> _displayColorList;
};

#endif // NXCOLORDISPLAYMODEL_H
