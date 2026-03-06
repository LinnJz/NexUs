#ifndef NXLOG_H
#define NXLOG_H

#include <QObject>

#include "LinnSingleton.h"
#include "NXProperty.h"

#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(Class)

class NXLogPrivate;

class NX_EXPORT NXLog : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXLog)
  Q_PROPERTY_CREATE_Q_H(QString, LogSavePath)
  Q_PROPERTY_CREATE_Q_H(QString, LogFileName)
  Q_PROPERTY_CREATE_Q_H(bool, IsLogFileNameWithTime)
  LINN_SINGLETON_CREATE(LINN_SINGLETON_UNIQUE(NXLog))

private:
  explicit NXLog(QObject *parent = nullptr);
  ~NXLog();

public:
  void initMessageLog(bool isEnable);
Q_SIGNALS:
  void logMessage(const QString& log);
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXLOG_H
