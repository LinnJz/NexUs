#ifndef NXMULTISELECTCOMBOBOX_H
#define NXMULTISELECTCOMBOBOX_H
#include <QComboBox>

#include "NXProperty.h"
class NXMultiSelectComboBoxPrivate;

class NX_EXPORT NXMultiSelectComboBox : public QComboBox
{
  Q_OBJECT
  Q_Q_CREATE(NXMultiSelectComboBox)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(bool, ShowCheckBox)

public:
  explicit NXMultiSelectComboBox(QWidget *parent = nullptr);
  ~NXMultiSelectComboBox();
  void setCurrentSelection(const QString& selection) noexcept;
  void setCurrentSelection(const QStringList& selection) noexcept;
  void setCurrentSelection(int index) noexcept;
  void setCurrentSelection(const QList<int>& selectionIndex) noexcept;
  QStringList getCurrentSelection() const noexcept;
  QList<int> getCurrentSelectionIndex() const noexcept;
Q_SIGNALS:
  void itemSelectionChanged(const QList<bool>& itemSelection);
  void currentTextListChanged(const QStringList& selectedTextList);

protected:
  void paintEvent(QPaintEvent *e) override;
  void showPopup() override;
  void hidePopup() override;
};

#endif // NXMULTISELECTCOMBOBOX_H
