#ifndef NXLOG_H
#define NXLOG_H

#include <QObject>

#include "LinnSingleton.h"
#include "NXProperty.h"
#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(CLASS)

class NXLogPrivate;

class NX_EXPORT NXLog : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXLog)
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXLog))
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), LogSavePath)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), LogFileName)
  Q_PROPERTY_CREATE_H(bool, IsLogFileNameWithTime)

private:
  explicit NXLog(QObject *parent = nullptr);
  ~NXLog();

public:
  void initMessageLog(bool isEnable);
  Q_SIGNAL void logMessage(const QString &log);
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXLOG_H
