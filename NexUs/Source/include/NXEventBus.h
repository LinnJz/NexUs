#ifndef NXEVENTBUS_H
#define NXEVENTBUS_H

#include <QObject>
#include <QVariantMap>

#include "LinnSingleton.h"
#include "NXDef.h"
class NXEventPrivate;

class NX_EXPORT NXEvent : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXEvent)
  Q_PROPERTY_CREATE_Q_H(QString, EventName);
  Q_PROPERTY_CREATE_Q_H(QString, FunctionName);
  Q_PROPERTY_CREATE_Q_H(Qt::ConnectionType, ConnectionType);

public:
  explicit NXEvent(QObject *parent = nullptr);
  explicit NXEvent(const QString& eventName, const QString& functionName, QObject *parent = nullptr);
  ~NXEvent() override;
  NXEventBusType::EventBusReturnType registerAndInit();
};

#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(Class)

class NXEventBusPrivate;

class NX_EXPORT NXEventBus : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXEventBus)
  LINN_SINGLETON_CREATE(LINN_SINGLETON_UNIQUE(NXEventBus))

private:
  explicit NXEventBus(QObject *parent = nullptr);
  ~NXEventBus() override;

public:
  NXEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});
  QStringList getRegisteredEventsName() const;

private:
  friend class NXEvent;
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXEVENTBUS_H
