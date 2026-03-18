#include "NXRollerPickerPrivate.h"

#include <QPropertyAnimation>
#include "NXRollerPicker.h"

NXRollerPickerPrivate::NXRollerPickerPrivate(QObject *parent)
    : QObject(parent)
{
}

NXRollerPickerPrivate::~NXRollerPickerPrivate() { }

void NXRollerPickerPrivate::onRollerPickerClicked() noexcept
{
  Q_Q(NXRollerPicker);
  QPoint targetPos(q->mapToGlobal(QPoint(
      -6, (q->height() - _rollerPickerContainer->height() + _rollerPickerContainer->getButtonAreaHeight()) / 2)));
  _rollerPickerContainer->show();
  _rollerPickerContainer->setGeometry(
      QRect(targetPos,
            QSize(_getRollerTotalWidth() + 16, _pickerRollerHeight + _rollerPickerContainer->getButtonAreaHeight())));
  _rollerPickerContainer->doPickerAnimation();
}

void NXRollerPickerPrivate::onOverButtonClicked() noexcept
{
  Q_Q(NXRollerPicker);
  Q_EMIT q->currentDataChanged(q->getCurrentData());
}

void NXRollerPickerPrivate::onCancelButtonClicked() noexcept { }

int NXRollerPickerPrivate::_getRollerTotalWidth() const noexcept
{
  int totalWidth = 0;
  for (auto rollerItem : _rollerPickerContainer->_rollerList) { totalWidth += rollerItem->width(); }
  return totalWidth;
}
