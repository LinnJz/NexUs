#include "NXCentralStackedWidgetPrivate.h"

#include <QStackedWidget>

NXCentralStackedWidgetPrivate::NXCentralStackedWidgetPrivate(QObject *parent)
    : QObject { parent }
{
}

NXCentralStackedWidgetPrivate::~NXCentralStackedWidgetPrivate() { }

void NXCentralStackedWidgetPrivate::onThemeModeChanged(NXThemeType::ThemeMode themeMode) noexcept
{
  _themeMode = themeMode;
}

void NXCentralStackedWidgetPrivate::_getCurrentStackPix() noexcept
{
  _targetStackPix    = QPixmap();
  bool isTransparent = _isTransparent;
  _isTransparent     = true;
  _containerStackedWidget->currentWidget()->setVisible(true);
  _currentStackPix = _containerStackedWidget->grab();
  _containerStackedWidget->currentWidget()->setVisible(false);
  _isTransparent = isTransparent;
}

void NXCentralStackedWidgetPrivate::_getTargetStackPix() noexcept
{
  _targetStackPix    = QPixmap();
  bool isTransparent = _isTransparent;
  _isTransparent     = true;
  _targetStackPix    = _containerStackedWidget->grab();
  _isTransparent     = isTransparent;
}
