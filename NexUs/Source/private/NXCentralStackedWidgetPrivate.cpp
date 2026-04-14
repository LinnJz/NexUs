#include "NXCentralStackedWidgetPrivate.h"

#include "NXLazyStackedWidget.h"

NXCentralStackedWidgetPrivate::NXCentralStackedWidgetPrivate(QObject *parent)
    : QObject { parent }
{
}

NXCentralStackedWidgetPrivate::~NXCentralStackedWidgetPrivate()
{
}

void
NXCentralStackedWidgetPrivate::onThemeModeChanged(NXThemeType::ThemeMode themeMode) noexcept
{
  _themeMode = themeMode;
}

void
NXCentralStackedWidgetPrivate::_getCurrentStackPix() noexcept
{
  _targetStackPix        = QPixmap();
  bool isTransparent     = _isTransparent;
  _isTransparent         = true;
  QWidget *currentWidget = _containerStackedWidget->currentWidget();
  if (!currentWidget)
  {
    _isTransparent = isTransparent;
    return;
  }
  currentWidget->setVisible(true);
  _currentStackPix = _containerStackedWidget->grab();
  currentWidget->setVisible(false);
  _isTransparent = isTransparent;
}

void
NXCentralStackedWidgetPrivate::_getTargetStackPix() noexcept
{
  _targetStackPix    = QPixmap();
  bool isTransparent = _isTransparent;
  _isTransparent     = true;
  _targetStackPix    = _containerStackedWidget->grab();
  _isTransparent     = isTransparent;
}
