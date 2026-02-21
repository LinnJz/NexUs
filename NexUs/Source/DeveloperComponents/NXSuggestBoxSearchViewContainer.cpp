#include "NXSuggestBoxSearchViewContainer.h"

#include <QPainter>

#include "NXTheme.h"

NXSuggestBoxSearchViewContainer::NXSuggestBoxSearchViewContainer(QWidget *parent)
    : QWidget { parent }
{
  setContentsMargins(8, 8, 8, 8);
  setObjectName("NXSuggestBoxSearchViewBaseWidget");
  setStyleSheet("#NXSuggestBoxSearchViewBaseWidget{background-color:transparent}");
  _themeMode = nxTheme->getThemeMode();
  connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

NXSuggestBoxSearchViewContainer::~NXSuggestBoxSearchViewContainer() { }

void NXSuggestBoxSearchViewContainer::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.save();
  painter.setRenderHints(QPainter::Antialiasing);
  nxTheme->drawEffectShadow(&painter, rect(), 6, 8);
  painter.setPen(NXThemeColor(_themeMode, PopupBorder));
  painter.setBrush(NXThemeColor(_themeMode, PopupBase));
  QRect foregroundRect(6, 0, rect().width() - 2 * 6, rect().height() - 6);
  painter.drawRoundedRect(foregroundRect, 8, 8);
  painter.restore();
}
