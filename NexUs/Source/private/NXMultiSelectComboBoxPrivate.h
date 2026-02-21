#ifndef NXMULTISELECTCOMBOBOXPRIVATE_H
#define NXMULTISELECTCOMBOBOXPRIVATE_H
#include <QList>
#include <QObject>

#include "NXDef.h"
#include "NXProperty.h"
class NXComboBoxStyle;
class NXComboBoxView;
class NXMultiSelectComboBox;
class NXMultiSelectComboBoxDelegate;

class NXMultiSelectComboBoxPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXMultiSelectComboBox)
  Q_PROPERTY_CREATE(qreal, ExpandIconRotate)
  Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(bool, ShowCheckBox)
public:
  explicit NXMultiSelectComboBoxPrivate(QObject *parent = nullptr);
  ~NXMultiSelectComboBoxPrivate() override;
  Q_SLOT void onItemPressed(const QModelIndex& index);

private:
  bool _isFirstPopup { false };
  bool _isAllowHidePopup { false };
  NXComboBoxStyle *_comboBoxStyle { nullptr };
  NXComboBoxView *_comboView { nullptr };
  NXMultiSelectComboBoxDelegate *_delegate { nullptr };
  QList<bool> _itemSelection;
  QString _currentText;
  QStringList _selectedTextList;
  void _refreshCurrentIndexs();
  void _adjustSelectedVector();
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXMULTISELECTCOMBOBOXPRIVATE_H
