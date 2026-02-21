#include "NXLCDNumber.h"
#include "NXLCDNumberPrivate.h"
#include "NXTheme.h"

NXLCDNumberPrivate::NXLCDNumberPrivate(QObject *parent)
    : QObject(parent)
{
}

NXLCDNumberPrivate::~NXLCDNumberPrivate() { }

void NXLCDNumberPrivate::onThemeModeChanged(NXThemeType::ThemeMode themeMode)
{
  Q_Q(NXLCDNumber);
  _themeMode       = themeMode;
  QPalette palette = q->palette();
  palette.setColor(QPalette::WindowText, NXThemeColor(themeMode, BasicText));
  q->setPalette(palette);
}
