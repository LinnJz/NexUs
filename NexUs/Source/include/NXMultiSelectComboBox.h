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
  void setCurrentSelection(const QString &selection);
  void setCurrentSelection(const QStringList &selection);
  void setCurrentSelection(int index);
  void setCurrentSelection(const QList<int> &selectionIndex);
  QStringList getCurrentSelection() const;
  QList<int> getCurrentSelectionIndex() const;
  Q_SIGNAL void itemSelectionChanged(const QVector<bool> &itemSelection);
  Q_SIGNAL void currentTextListChanged(const QStringList &selectedTextList);

protected:
  void paintEvent(QPaintEvent *e) override;
  void showPopup() override;
  void hidePopup() override;

private:
  friend class NXMultiSelectComboBoxPrivate;
  void _resetIndicatorAnimations();
};

#endif // NXMULTISELECTCOMBOBOX_H
