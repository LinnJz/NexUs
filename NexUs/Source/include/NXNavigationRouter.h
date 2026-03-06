#ifndef NXNAVIGATIONROUTER_H
#define NXNAVIGATIONROUTER_H

#include <QObject>
#include <QVariantMap>

#include "LinnSingleton.h"
#include "NXDef.h"

#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(Class)

class NXNavigationRouterPrivate;

class NX_EXPORT NXNavigationRouter : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXNavigationRouter)
  Q_PROPERTY_CREATE_Q_H(int, MaxRouteCount)
  LINN_SINGLETON_CREATE(LINN_SINGLETON_UNIQUE(NXNavigationRouter))

private:
  explicit NXNavigationRouter(QObject *parent = nullptr);
  ~NXNavigationRouter() override;

Q_SIGNALS:
  Q_SIGNAL void navigationRouterStateChanged(NXNavigationRouterType::RouteMode routeMode);

public:
  NXNavigationRouterType::NavigationRouteType navigationRoute(QObject *routeObject,
                                                              QString routeFunctionName,
                                                              const QVariantMap& routeData      = {},
                                                              Qt::ConnectionType connectionType = Qt::AutoConnection);
  void clearNavigationRoute();
  void navigationRouteBack();
  void navigationRouteForward();
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXNAVIGATIONROUTER_H
