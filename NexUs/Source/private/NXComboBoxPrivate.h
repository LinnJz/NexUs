#ifndef NXCOMBOBOXPRIVATE_H
#define NXCOMBOBOXPRIVATE_H

#include <QObject>

#include "NXDef.h"

class QLineEdit;
class NXComboBox;
class NXComboBoxStyle;

class NXComboBoxPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXComboBox);
  Q_PROPERTY_CREATE_D(int, BorderRadius)

public:
  explicit NXComboBoxPrivate(QObject *parent = nullptr);
  ~NXComboBoxPrivate() override;

  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  NXThemeType::ThemeMode _themeMode;
  bool _isAllowHidePopup { false };
  NXComboBoxStyle *_comboBoxStyle { nullptr };
};

#endif // NXCOMBOBOXPRIVATE_H
