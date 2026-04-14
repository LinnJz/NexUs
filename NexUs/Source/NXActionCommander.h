#ifndef NXACTIONCOMMANDER_H
#define NXACTIONCOMMANDER_H

#include <QObject>
#include <QVariantMap>

#include "LinnSingleton.h"
#include "NXDef.h"

class NX_EXPORT NXActionCommand : public QObject
{
  Q_OBJECT

public:
  explicit NXActionCommand(QObject *parent = nullptr);
  ~NXActionCommand() override;

  virtual void undo() = 0;
  virtual void redo() = 0;
};

#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(Class)

class NXActionCommanderPrivate;

class NX_EXPORT NXActionCommander : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXActionCommander)
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXActionCommander))
  Q_PROPERTY_CREATE_H(int, MaxRouteCount)

private:
  explicit NXActionCommander(QObject *parent = nullptr);
  ~NXActionCommander() override;

Q_SIGNALS:
  Q_SIGNAL void commanderStateChanged(const QString &domainName, NXActionCommanderType::CommanderState state);

public:
  void recordCommand(const QString &domainName, NXActionCommand *command, bool isRedo = true) noexcept;
  void clearCommand(const QString &domainName) noexcept;
  void undoCommand(const QString &domainName) noexcept;
  void redoCommand(const QString &domainName) noexcept;

  NXActionCommanderType::CommanderState getCommanderUndoState(const QString &domainName) const noexcept;
  NXActionCommanderType::CommanderState getCommanderRedoState(const QString &domainName) const noexcept;
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXACTIONCOMMANDER_H
