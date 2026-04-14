#ifndef NXTEXTPRIVATE_H
#define NXTEXTPRIVATE_H

#include <QColor>
#include <QObject>

#include "NXDef.h"

class NXText;

class NXTextPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXText)
  Q_PROPERTY_CREATE_D(bool, IsAllowClick)
  Q_PROPERTY_CREATE_D(bool, IsWrapAnywhere)
  Q_PROPERTY_CREATE_D(NXTextType::TextStyle, TextStyle)
  Q_PROPERTY_CREATE_D(NXIconType::IconName, NXIcon)

public:
  explicit NXTextPrivate(QObject *parent = nullptr);
  ~NXTextPrivate() override;
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode) noexcept;

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXTEXTPRIVATE_H
