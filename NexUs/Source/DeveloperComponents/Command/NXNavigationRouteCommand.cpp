#include "NXNavigationRouteCommand.h"

#include "NXNavigationBar.h"

NXNavigationRouteCommand::NXNavigationRouteCommand(QObject *parent)
    : NXActionCommand(parent)
{
}

NXNavigationRouteCommand::~NXNavigationRouteCommand()
{
}

void
NXNavigationRouteCommand::undo()
{
  _pNavigationBar->navigation(_pUndoPageKey, false, false);
}

void
NXNavigationRouteCommand::redo()
{
  _pNavigationBar->navigation(_pRedoPageKey, false, false);
}
