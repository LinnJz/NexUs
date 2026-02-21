#ifndef NXMULTISELECTCOMBOBOX_H
#define NXMULTISELECTCOMBOBOX_H
#include <QComboBox>

#include "NXProperty.h"
class NXMultiSelectComboBoxPrivate;

class NX_EXPORT NXMultiSelectComboBox : public QComboBox
{
  Q_OBJECT
  Q_Q_CREATE(NXMultiSelectComboBox)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(bool, ShowCheckBox)
public:
  explicit NXMultiSelectComboBox(QWidget *parent = nullptr);
  ~NXMultiSelectComboBox();
  void setCurrentSelection(const QString& selection);
  void setCurrentSelection(QStringList selection);
  void setCurrentSelection(int index);
  void setCurrentSelection(QList<int> selectionIndex);
  QStringList getCurrentSelection() const;
  QList<int> getCurrentSelectionIndex() const;
Q_SIGNALS:
  Q_SIGNAL void itemSelectionChanged(QList<bool> itemSelection);
  Q_SIGNAL void currentTextListChanged(QStringList selectedTextList);

protected:
  void paintEvent(QPaintEvent *e) override;
  void showPopup() override;
  void hidePopup() override;
};

#endif // NXMULTISELECTCOMBOBOX_H
