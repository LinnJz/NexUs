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
  Q_PROPERTY_CREATE_H(int, MaxRouteCount)
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXNavigationRouter))

private:
  explicit NXNavigationRouter(QObject *parent = nullptr);
  ~NXNavigationRouter() override;

Q_SIGNALS:
  Q_SIGNAL void navigationRouterStateChanged(NXNavigationRouterType::RouteMode routeMode);
  Q_SIGNAL void windowRouterStateChanged(QObject *context, NXNavigationRouterType::RouteMode routeMode);

public:
  // 全局路由（向后兼容）
  NXNavigationRouterType::NavigationRouteType
  navigationRoute(QObject *routeObject,
                  QString routeFunctionName,
                  const QVariantMap& routeData      = {},
                  Qt::ConnectionType connectionType = Qt::AutoConnection) noexcept;
  void clearNavigationRoute() noexcept;
  void navigationRouteBack() noexcept;
  void navigationRouteForward() noexcept;

  // 每窗口独立路由
  NXNavigationRouterType::NavigationRouteType
  navigationRoute(QObject *context,
                  QObject *routeObject,
                  QString routeFunctionName,
                  const QVariantMap& routeData      = {},
                  Qt::ConnectionType connectionType = Qt::AutoConnection) noexcept;
  void clearNavigationRoute(QObject *context) noexcept;
  void navigationRouteBack(QObject *context) noexcept;
  void navigationRouteForward(QObject *context) noexcept;
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXNAVIGATIONROUTER_H
