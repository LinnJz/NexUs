#include "NXLog.h"

#include <QDir>

#include "private/NXLogPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(NXLog, QString, LogSavePath)
Q_PROPERTY_CREATE_Q_CPP(NXLog, QString, LogFileName)
Q_PROPERTY_CREATE_Q_CPP(NXLog, bool, IsLogFileNameWithTime)

NXLog::NXLog(QObject *parent)
    : QObject { parent }
    , d_ptr(new NXLogPrivate())
{
  Q_D(NXLog);
  d->q_ptr                   = this;
  d->_pLogFileName           = "NXLog";
  d->_pLogSavePath           = QDir::currentPath();
  d->_pIsLogFileNameWithTime = false;
  d->_clearLogFile();
  connect(this, &NXLog::pLogSavePathChanged, d, &NXLogPrivate::_clearLogFile);
  connect(this, &NXLog::pLogFileNameChanged, d, &NXLogPrivate::_clearLogFile);
  connect(this, &NXLog::pIsLogFileNameWithTimeChanged, d, &NXLogPrivate::_clearLogFile);
}

NXLog::~NXLog() { }

void NXLog::initMessageLog(bool isEnable)
{
  Q_D(NXLog);
  qInstallMessageHandler(isEnable ? d->_messageLogHander : 0);
}
