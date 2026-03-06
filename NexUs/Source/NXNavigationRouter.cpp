#include "NXNavigationRouter.h"

#include "private/NXNavigationRouterPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(NXNavigationRouter, int, MaxRouteCount)

NXNavigationRouter::NXNavigationRouter(QObject *parent)
    : QObject { parent }
    , d_ptr(new NXNavigationRouterPrivate())
{
  Q_D(NXNavigationRouter);
  d->q_ptr           = this;
  d->_pMaxRouteCount = 25;
}

NXNavigationRouter::~NXNavigationRouter() { }

NXNavigationRouterType::NavigationRouteType NXNavigationRouter::navigationRoute(QObject *routeObject,
                                                                                QString routeFunctionName,
                                                                                const QVariantMap& routeData,
                                                                                Qt::ConnectionType connectionType)
{
  Q_D(NXNavigationRouter);
  if (!routeObject) { return NXNavigationRouterType::ObjectInvalid; }
  if (routeFunctionName.isEmpty()) { return NXNavigationRouterType::FunctionNameInvalid; }
  if (d->_routeList.count() == 0)
  {
    Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::BackValid);
    Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::ForwardInvalid);
  }
  else
  {
    if (d->_routeList.count() >= d->_pMaxRouteCount)
    {
      d->_routeList.removeFirst();
      d->_currentIndex -= 1;
    }
  }
  QVariantMap saveData;
  saveData.insert("NXRouteObject", QVariant::fromValue<QObject *>(routeObject));
  saveData.insert("NXRouteFunctionName", routeFunctionName);
  saveData.insert("NXRouteData", routeData);
  saveData.insert("NXRouteConnectionType", QVariant::fromValue<Qt::ConnectionType>(connectionType));
  if (d->_currentIndex != d->_routeList.count() - 1)
  {
    if (d->_currentIndex == -1) { d->_routeList.clear(); }
    else
    {
      d->_routeList.remove(d->_currentIndex + 1, d->_routeList.count() - d->_currentIndex - 1);
    }
    Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::ForwardInvalid);
  }
  if (d->_currentIndex <= 0)
  {
    Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::BackValid);
    Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::ForwardInvalid);
  }
  d->_routeList.append(saveData);
  d->_currentIndex = d->_routeList.count() - 1;
  return NXNavigationRouterType::Success;
}

void NXNavigationRouter::clearNavigationRoute()
{
  Q_D(NXNavigationRouter);
  d->_currentIndex = -1;
  d->_routeList.clear();
  Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::BackInvalid);
  Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::ForwardValid);
}

void NXNavigationRouter::navigationRouteBack()
{
  Q_D(NXNavigationRouter);
  if (d->_routeList.isEmpty()) { return; }
  if (d->_currentIndex == 0) { Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::BackInvalid); }
  if (d->_currentIndex == d->_routeList.size() - 1)
  {
    Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::ForwardValid);
  }
  QVariantMap routeSaveData = d->_routeList[d->_currentIndex];
  d->_currentIndex -= 1;
  QObject *routeObject      = routeSaveData.value("NXRouteObject").value<QObject *>();
  QString routeFunctionName = routeSaveData.value("NXRouteFunctionName").toString();
  QVariantMap routeData     = routeSaveData.value("NXRouteData").toMap();
  routeData.insert("NXRouteBackMode", true);
  Qt::ConnectionType connectionType = routeSaveData.value("NXRouteConnectionType").value<Qt::ConnectionType>();
  QMetaObject::invokeMethod(
      routeObject, routeFunctionName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
}

void NXNavigationRouter::navigationRouteForward()
{
  Q_D(NXNavigationRouter);
  if (d->_routeList.isEmpty()) { return; }
  if (d->_currentIndex <= 0) { Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::BackValid); }
  if (d->_currentIndex == d->_routeList.size() - 2)
  {
    Q_EMIT navigationRouterStateChanged(NXNavigationRouterType::ForwardInvalid);
  }
  if (d->_currentIndex < d->_routeList.size() - 1) { d->_currentIndex += 1; }
  QVariantMap routeSaveData = d->_routeList[d->_currentIndex];
  QObject *routeObject      = routeSaveData.value("NXRouteObject").value<QObject *>();
  QString routeFunctionName = routeSaveData.value("NXRouteFunctionName").toString();
  QVariantMap routeData     = routeSaveData.value("NXRouteData").toMap();
  routeData.insert("NXRouteBackMode", false);
  Qt::ConnectionType connectionType = routeSaveData.value("NXRouteConnectionType").value<Qt::ConnectionType>();
  QMetaObject::invokeMethod(
      routeObject, routeFunctionName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
}
