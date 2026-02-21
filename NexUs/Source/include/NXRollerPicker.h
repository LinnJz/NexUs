#ifndef NXFRAMEWORK_NXROLLERPICKER_H
#define NXFRAMEWORK_NXROLLERPICKER_H

#include <QPushButton>
#include "NXProperty.h"

class NXRollerPickerPrivate;

class NX_EXPORT NXRollerPicker : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXRollerPicker)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
  explicit NXRollerPicker(QWidget *parent = nullptr);
  ~NXRollerPicker() override;

  void addRoller(const QStringList& itemList, bool isEnableLoop = true);
  void removeRoller(int index);

  void setRollerItemList(int index, const QStringList& itemList);
  void setRollerWidth(int index, int width);

  void setCurrentData(int index, const QString& data);
  void setCurrentData(const QStringList& dataList);
  QString getCurrentData(int index) const;
  QStringList getCurrentData() const;

  void setCurrentIndex(int rollerIndex, int index);
  void setCurrentIndex(const QList<int>& indexList);
  int getCurrentIndex(int rollerIndex) const;
  QList<int> getCurrentIndex() const;

Q_SIGNALS:
  Q_SIGNAL void currentDataChanged(const QStringList& dataList);
  Q_SIGNAL void currentDataSelectionChanged(const QStringList& dataList);

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXFRAMEWORK_NXROLLERPICKER_H
