#ifndef NXFRAMEWORK_NXROLLERPICKER_H
#define NXFRAMEWORK_NXROLLERPICKER_H

#include <QPushButton>
#include "NXProperty.h"

class NXRollerPickerPrivate;

class NX_EXPORT NXRollerPicker : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXRollerPicker)
  Q_PROPERTY_CREATE_H(int, BorderRadius)

public:
  explicit NXRollerPicker(QWidget *parent = nullptr);
  ~NXRollerPicker() override;

  void addRoller(const QStringList& itemList, bool isEnableLoop = true) noexcept;
  void removeRoller(int index) noexcept;

  void setRollerItemList(int index, const QStringList& itemList) noexcept;
  void setRollerWidth(int index, int width) noexcept;

  void setCurrentData(int index, const QString& data) noexcept;
  void setCurrentData(const QStringList& dataList) noexcept;
  QString getCurrentData(int index) const noexcept;
  QStringList getCurrentData() const noexcept;

  void setCurrentIndex(int rollerIndex, int index) noexcept;
  void setCurrentIndex(const QList<int>& indexList) noexcept;
  int getCurrentIndex(int rollerIndex) const noexcept;
  QList<int> getCurrentIndex() const noexcept;

Q_SIGNALS:
  void currentDataChanged(const QStringList& dataList);
  void currentDataSelectionChanged(const QStringList& dataList);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXFRAMEWORK_NXROLLERPICKER_H
