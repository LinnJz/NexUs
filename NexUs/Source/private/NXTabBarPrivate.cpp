#include "NXTabBarPrivate.h"

#include <QHBoxLayout>

#include "DeveloperComponents/NXTabBarStyle.h"
#include "NXTabBar.h"

NXTabBarPrivate::NXTabBarPrivate(QObject *parent)
    : QObject { parent }
{
}

NXTabBarPrivate::~NXTabBarPrivate() { }

void NXTabBarPrivate::_updateFixedAreaLayout()
{
  Q_Q(NXTabBar);
  bool hasLeft  = _leftFixedLayout->count() > 0;
  bool hasRight = _rightFixedLayout->count() > 0;
  if (!hasLeft && !hasRight) return;
  _leftFixedContainer->setVisible(hasLeft);
  _rightFixedContainer->setVisible(hasRight);

  int leftWidth           = hasLeft ? _leftFixedContainer->sizeHint().width() : 0;
  int rightWidth          = hasRight ? _rightFixedContainer->sizeHint().width() : 0;
  int separatorSpaceLeft  = hasLeft ? 10 : 0;
  int separatorSpaceRight = hasRight ? 10 : 0;
  q->setContentsMargins(leftWidth + separatorSpaceLeft, 0, rightWidth + separatorSpaceRight, 0);

  if (hasLeft)
  {
    _leftFixedContainer->setGeometry(0, 0, leftWidth, q->height());
    _leftFixedContainer->raise();
  }
  if (hasRight)
  {
    _rightFixedContainer->setGeometry(q->width() - rightWidth, 0, rightWidth, q->height());
    _rightFixedContainer->raise();
  }
}

