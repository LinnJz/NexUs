#ifndef NXACTIONCOMMANDER_H
#define NXACTIONCOMMANDER_H

#include <QObject>
#include <QVariantMap>

#include "LinnSingleton.h"
#include "NXDef.h"
#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(CLASS)

class NX_EXPORT NXActionCommand : public QObject
{
  Q_OBJECT

public:
  explicit NXActionCommand(QObject *parent = nullptr);
  ~NXActionCommand();

  virtual void undo() = 0;
  virtual void redo() = 0;
};

class NXActionCommanderPrivate;

class NX_EXPORT NXActionCommander : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXActionCommander)
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXActionCommander))
  Q_PROPERTY_CREATE_H(int, MaxRouteCount)

private:
  explicit NXActionCommander(QObject *parent = nullptr);
  ~NXActionCommander();
  Q_SIGNAL void commanderStateChanged(const QString &domainName, NXActionCommanderType::CommanderState state);

public:
  void recordCommand(const QString &domainName, NXActionCommand *command, bool isRedo = true);
  void clearCommand(const QString &domainName);
  void undoCommand(const QString &domainName);
  void redoCommand(const QString &domainName);

  NXActionCommanderType::CommanderState getCommanderUndoState(const QString &domainName) const;
  NXActionCommanderType::CommanderState getCommanderRedoState(const QString &domainName) const;
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXACTIONCOMMANDER_H
