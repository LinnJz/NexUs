#include "NXComboBox.h"
#include "NXComboBoxPrivate.h"
#include "NXTheme.h"

#include <QLineEdit>

NXComboBoxPrivate::NXComboBoxPrivate(QObject *parent)
    : QObject { parent }
{
}

NXComboBoxPrivate::~NXComboBoxPrivate() { }

void NXComboBoxPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode)
{
  Q_Q(NXComboBox);
  _themeMode    = themeMode;
  auto lineEdit = q->lineEdit();
  if (lineEdit)
  {
    QPalette palette = lineEdit->palette();
    palette.setColor(QPalette::Text, NXThemeColor(_themeMode, BasicText));
    palette.setColor(QPalette::PlaceholderText,
                     _themeMode == NXThemeType::Light ? QColor(0x00, 0x00, 0x00, 128) : QColor(0xBA, 0xBA, 0xBA));
    lineEdit->setPalette(palette);
  }
}
