#ifndef NXMULTISELECTCOMBOBOXPRIVATE_H
#define NXMULTISELECTCOMBOBOXPRIVATE_H
#include <QObject>
#include <QVector>

#include "NXDef.h"
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
  ~NXMultiSelectComboBoxPrivate();
  Q_SLOT void onItemPressed(const QModelIndex &index);

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  bool _isFirstPopup { false };
  bool _isAllowHidePopup { false };
  NXThemeType::ThemeMode _themeMode;
  QVector<bool> _itemSelection;
  QString _currentText;
  QStringList _selectedTextList;
  NXComboBoxStyle *_comboBoxStyle { nullptr };
  NXComboBoxView *_comboView { nullptr };
  NXMultiSelectComboBoxDelegate *_delegate { nullptr };
  void _refreshCurrentIndexs();
  void _adjustSelectedVector();
};

#endif // NXMULTISELECTCOMBOBOXPRIVATE_H
