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

  qreal _textSpacing { 0.5 };
  QColor _borderColor;
  NXThemeType::ThemeMode _themeMode;
  int _borderPx { 1 };
  Q_PROPERTY_CREATE_D(NXTextType::TextStyle, TextStyle)
  Q_PROPERTY_CREATE_D(NXIconType::IconName, NXIcon)
  Q_PROPERTY_CREATE_D(NXWidgetType::BorderFlags, BorderFlag)
  Q_PROPERTY_CREATE_D(bool, IsAllowClick)
  Q_PROPERTY_CREATE_D(bool, IsWrapAnywhere)

public:
  explicit NXTextPrivate(QObject *parent = nullptr);
  ~NXTextPrivate() override;
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);
};

#endif // NXTEXTPRIVATE_H
