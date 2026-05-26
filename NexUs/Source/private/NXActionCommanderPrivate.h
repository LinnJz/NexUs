#ifndef NXACTIONCOMMANDERPRIVATE_H
#define NXACTIONCOMMANDERPRIVATE_H
#include <QMap>
#include <QObject>
#include <QVector>

#include "NXDef.h"
class NXActionCommand;
class NXActionCommander;

class NXActionCommanderPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXActionCommander)
  Q_PROPERTY_CREATE_D(int, MaxRouteCount)

public:
  struct NXActionCommandDomainData
  {
    int currentIndex { -1 };
    NXActionCommanderType::CommanderState undoState { NXActionCommanderType::UndoInvalid };
    NXActionCommanderType::CommanderState redoState { NXActionCommanderType::RedoInvalid };
    QVector<NXActionCommand *> commandList;
  };

  explicit NXActionCommanderPrivate(QObject *parent = nullptr);
  ~NXActionCommanderPrivate();

private:
  QMap<QString, NXActionCommandDomainData> _commandDomainMap;
};

#endif // NXACTIONCOMMANDERPRIVATE_H
