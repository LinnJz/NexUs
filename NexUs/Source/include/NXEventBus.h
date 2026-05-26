#ifndef NXEVENTBUS_H
#define NXEVENTBUS_H

#include <QObject>
#include <QVariantMap>

#include "LinnSingleton.h"
#include "NXDef.h"
#include "NXProperty.h"
#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(CLASS)
class NXEventPrivate;

class NX_EXPORT NXEvent : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXEvent)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), EventName)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), FunctionName)
  Q_PROPERTY_CREATE_H(Qt::ConnectionType, ConnectionType)

public:
  explicit NXEvent(QObject *parent = nullptr);
  explicit NXEvent(QString eventName, QString functionName, QObject *parent = nullptr);
  ~NXEvent();
  NXEventBusType::EventBusReturnType registerAndInit();
};

class NXEventBusPrivate;

class NX_EXPORT NXEventBus : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXEventBus)
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXEventBus))

private:
  explicit NXEventBus(QObject *parent = nullptr);
  ~NXEventBus();

public:
  NXEventBusType::EventBusReturnType post(const QString &eventName, const QVariantMap &data = {});
  QStringList getRegisteredEventsName() const;

private:
  friend class NXEvent;
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXEVENTBUS_H
