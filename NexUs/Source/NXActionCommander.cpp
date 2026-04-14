#include "NXActionCommander.h"

#include "NXActionCommanderPrivate.h"

NXActionCommand::NXActionCommand(QObject *parent)
    : QObject(parent)
{
}

NXActionCommand::~NXActionCommand()
{
}

Q_PROPERTY_CREATE_CPP(NXActionCommander, int, MaxRouteCount)

NXActionCommander::NXActionCommander(QObject *parent)
    : QObject { parent }
    , d_ptr(new NXActionCommanderPrivate())
{
  Q_D(NXActionCommander);
  d->q_ptr           = this;
  d->_pMaxRouteCount = 500;
}

NXActionCommander::~NXActionCommander()
{
}

void
NXActionCommander::recordCommand(const QString &domainName, NXActionCommand *command, bool isRedo) noexcept
{
  Q_D(NXActionCommander);
  if (!command)
  {
    return;
  }
  command->setParent(this);
  auto &commandData = d->_commandDomainMap[domainName];
  auto &commandList = commandData.commandList;
  if (commandData.currentIndex <= 0 || commandList.count() == 0)
  {
    commandData.undoState = NXActionCommanderType::UndoValid;
    commandData.redoState = NXActionCommanderType::RedoInvalid;
    Q_EMIT commanderStateChanged(domainName, NXActionCommanderType::UndoValid);
    Q_EMIT commanderStateChanged(domainName, NXActionCommanderType::RedoInvalid);
  }
  else
  {
    // 超过最大命令数 则移除第一条命令
    if (commandList.count() >= d->_pMaxRouteCount)
    {
      commandList.first()->deleteLater();
      commandList.removeFirst();
      commandData.currentIndex -= 1;
    }
  }
  // 当前索引不位于末尾 则清除索引后的数据
  if (commandData.currentIndex != commandList.count() - 1)
  {
    for (int i = commandData.currentIndex + 1; i < commandList.count() - commandData.currentIndex - 1; i++)
    {
      commandList.at(i)->deleteLater();
    }
    commandList.remove(commandData.currentIndex + 1, commandList.count() - commandData.currentIndex - 1);
    if (commandData.currentIndex > 0)
    {
      commandData.redoState = NXActionCommanderType::RedoInvalid;
      Q_EMIT commanderStateChanged(domainName, NXActionCommanderType::RedoInvalid);
    }
  }
  commandList.append(command);
  commandData.currentIndex = commandList.count() - 1;
  if (isRedo)
  {
    // 加入时Redo一次
    command->redo();
  }
}

void
NXActionCommander::clearCommand(const QString &domainName) noexcept
{
  Q_D(NXActionCommander);
  if (!d->_commandDomainMap.contains(domainName))
  {
    return;
  }
  auto &commandData        = d->_commandDomainMap[domainName];
  auto &commandList        = commandData.commandList;
  commandData.currentIndex = -1;
  for (const auto command : commandList)
  {
    command->deleteLater();
  }
  commandList.clear();
  commandData.undoState = NXActionCommanderType::UndoInvalid;
  commandData.redoState = NXActionCommanderType::RedoInvalid;
  Q_EMIT commanderStateChanged(domainName, NXActionCommanderType::UndoInvalid);
  Q_EMIT commanderStateChanged(domainName, NXActionCommanderType::RedoInvalid);
}

void
NXActionCommander::undoCommand(const QString &domainName) noexcept
{
  Q_D(NXActionCommander);
  if (!d->_commandDomainMap.contains(domainName))
  {
    return;
  }
  auto &commandData = d->_commandDomainMap[domainName];
  if (commandData.undoState == NXActionCommanderType::UndoInvalid)
  {
    return;
  }
  auto &commandList = commandData.commandList;
  if (commandList.isEmpty())
  {
    return;
  }
  if (commandData.currentIndex == 0)
  {
    commandData.undoState = NXActionCommanderType::UndoInvalid;
    Q_EMIT commanderStateChanged(domainName, NXActionCommanderType::UndoInvalid);
  }
  if (commandData.currentIndex == commandList.size() - 1)
  {
    commandData.redoState = NXActionCommanderType::RedoValid;
    Q_EMIT commanderStateChanged(domainName, NXActionCommanderType::RedoValid);
  }
  auto command = commandList[commandData.currentIndex];
  command->undo();
  commandData.currentIndex -= 1;
}

void
NXActionCommander::redoCommand(const QString &domainName) noexcept
{
  Q_D(NXActionCommander);
  if (!d->_commandDomainMap.contains(domainName))
  {
    return;
  }
  auto &commandData = d->_commandDomainMap[domainName];
  if (commandData.redoState == NXActionCommanderType::RedoInvalid)
  {
    return;
  }
  auto &commandList = commandData.commandList;
  if (commandList.isEmpty())
  {
    return;
  }
  if (commandData.currentIndex <= 0)
  {
    commandData.undoState = NXActionCommanderType::UndoValid;
    Q_EMIT commanderStateChanged(domainName, NXActionCommanderType::UndoValid);
  }
  if (commandData.currentIndex == commandList.size() - 2)
  {
    commandData.redoState = NXActionCommanderType::RedoInvalid;
    Q_EMIT commanderStateChanged(domainName, NXActionCommanderType::RedoInvalid);
  }
  if (commandData.currentIndex < commandList.size() - 1)
  {
    commandData.currentIndex += 1;
  }
  auto command = commandList[commandData.currentIndex];
  command->redo();
}

NXActionCommanderType::CommanderState
NXActionCommander::getCommanderUndoState(const QString &domainName) const noexcept
{
  Q_D(const NXActionCommander);
  if (!d->_commandDomainMap.contains(domainName))
  {
    return NXActionCommanderType::UndoInvalid;
  }
  return d->_commandDomainMap[domainName].undoState;
}

NXActionCommanderType::CommanderState
NXActionCommander::getCommanderRedoState(const QString &domainName) const noexcept
{
  Q_D(const NXActionCommander);
  if (!d->_commandDomainMap.contains(domainName))
  {
    return NXActionCommanderType::RedoInvalid;
  }
  return d->_commandDomainMap[domainName].redoState;
}
