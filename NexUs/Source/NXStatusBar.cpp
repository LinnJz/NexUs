#include "NXStatusBar.h"

#include <QPainter>
#include <QTimer>

#include "DeveloperComponents/NXStatusBarStyle.h"

NXStatusBar::NXStatusBar(QWidget *parent)
    : QStatusBar(parent)
{
  setObjectName("NXStatusBar");
  setStyleSheet("#NXStatusBar{background-color:transparent;}");
  setFixedHeight(28);
  setContentsMargins(20, 0, 0, 0);
  setStyle(new NXStatusBarStyle(style()));
}

NXStatusBar::~NXStatusBar() { }
