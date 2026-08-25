#ifndef NXACTIONCOMMANDERPRIVATE_H
#define NXACTIONCOMMANDERPRIVATE_H
#include <QMap>
#include <QObject>
#include <QVector>

#include "NXDef.h"
class NXActionCommand;
class NXActionCommander;
class NXText;
class NXProgressRing;
class NXActionCommanderModel;
class NXActionCommanderView;

class NXActionCommanderPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXActionCommander)
  Q_PROPERTY_CREATE_D(int, IsMessageDisplayEnable)
  Q_PROPERTY_CREATE_D(int, MessageDisplayMesc)
  Q_PROPERTY_CREATE_D(int, MaxCommandCount)
  Q_PROPERTY_CREATE_D(bool, IsRunToCommandProcessEventEnable)

public:
  struct NXActionCommandDomainData
  {
    bool isBusyMode { false };
    int currentIndex { -1 };
    NXActionCommanderType::CommanderState undoState { NXActionCommanderType::UndoInvalid };
    NXActionCommanderType::CommanderState redoState { NXActionCommanderType::RedoInvalid };
    NXActionCommanderModel *commanderModel { nullptr };
    QVector<NXActionCommand *> commandList;
  };

  explicit NXActionCommanderPrivate(QObject *parent = nullptr);
  ~NXActionCommanderPrivate();

private:
  void _showCommandDesc(const NXActionCommand *command, bool isRedo, bool isExecute = false) const;
  bool _executeCommand(const QString &domainName, NXActionCommand *command);
  bool _recordCommand(const QString &domainName, NXActionCommand *command, bool isRedo);
  void _undoCommand(const QString &domainName, bool isShowMessage);
  void _redoCommand(const QString &domainName, bool isShowMessage);
  void _initDomain(NXActionCommandDomainData &domainData) const;
  bool _isCommanderValid { true }; // 是否处于可编辑模式
  NXProgressRing *_commanderBusyRing { nullptr };
  NXText *_commanderBusyRingValue { nullptr };
  QWidget *_commanderDesign { nullptr };
  NXActionCommanderView *_commanderView { nullptr };
  QMap<QString, NXActionCommandDomainData> _commandDomainMap;
};

#endif // NXACTIONCOMMANDERPRIVATE_H
