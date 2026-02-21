#ifndef NXLOG_H
#define NXLOG_H

#include <QObject>

#include "NXProperty.h"
#include "singleton.h"

class NXLogPrivate;

class NX_EXPORT NXLog : public QObject
{
#pragma push_macro("DISABLE_COPY")
#undef DISABLE_COPY
#define DISABLE_COPY(Class)
  SINGLETON_CREATE_H(NXLog)
#pragma pop_macro("DISABLE_COPY")
  Q_OBJECT
  Q_Q_CREATE(NXLog)
  Q_PROPERTY_CREATE_Q_H(QString, LogSavePath)
  Q_PROPERTY_CREATE_Q_H(QString, LogFileName)
  Q_PROPERTY_CREATE_Q_H(bool, IsLogFileNameWithTime)

private:
  explicit NXLog(QObject *parent = nullptr);
  ~NXLog();

public:
  void initMessageLog(bool isEnable);
Q_SIGNALS:
  void logMessage(const QString& log);
};

#endif // NXLOG_H
