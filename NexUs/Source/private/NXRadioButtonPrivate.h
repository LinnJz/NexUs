#ifndef NXRADIOBUTTONPRIVATE_H
#define NXRADIOBUTTONPRIVATE_H
#include <QObject>

#include "NXDef.h"
class NXRadioButton;

class NXRadioButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXRadioButton)
public:
  explicit NXRadioButtonPrivate(QObject *parent = nullptr);
  ~NXRadioButtonPrivate() override;
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  NXTextType::TextStyle _textStyle { NXTextType::NoStyle };
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXRADIOBUTTONPRIVATE_H
