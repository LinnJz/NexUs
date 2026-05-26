#include "NXWindowStackChangeCommand.h"

#include "NXCentralStackedWidget.h"
#include "NXWindow.h"

NXWindowStackChangeCommand::NXWindowStackChangeCommand(QObject *parent)
    : NXActionCommand(parent)
{
}

NXWindowStackChangeCommand::~NXWindowStackChangeCommand()
{
}

void
NXWindowStackChangeCommand::undo()
{
  _pWindowPrivate->_centerStackedWidget->doWindowStackSwitch(_pWindowPrivate->_pStackSwitchMode, _pUndoStackIndex,
                                                             false);
  Q_EMIT _pWindowPrivate->q_ptr->pCurrentStackIndexChanged();
}

void
NXWindowStackChangeCommand::redo()
{
  _pWindowPrivate->_centerStackedWidget->doWindowStackSwitch(_pWindowPrivate->_pStackSwitchMode, _pRedoStackIndex,
                                                             false);
  Q_EMIT _pWindowPrivate->q_ptr->pCurrentStackIndexChanged();
}
