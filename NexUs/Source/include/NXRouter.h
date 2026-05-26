#ifndef NXROUTER_H
#define NXROUTER_H

#include <functional>

#include <QObject>
#include <QVariantMap>

#include "LinnSingleton.h"
#include "NXDef.h"
#include "NXProperty.h"
#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(CLASS)
class NXWindow;
class NXRouterPrivate;

struct NX_EXPORT NXRouteConfig
{
  QString path;
  QString title;
  NXIconType::IconName icon = NXIconType::None;
  QVariantMap meta;
  std::function<QWidget *()> factory;
  QVector<NXRouteConfig> children;
  int keyPoints = 0;
};

using NXRouteGuard     = std::function<bool(const QString &to, const QString &from, const QVariantMap &params)>;
using NXRouteAfterHook = std::function<void(const QString &to, const QString &from, const QVariantMap &params)>;

class NX_EXPORT NXRouter : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXRouter)
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXRouter))

private:
  explicit NXRouter(QObject *parent = nullptr);
  ~NXRouter();

public:
  void bindWindow(NXWindow *window);
  NXWindow *getBoundWindow() const;

  NXRouterType::NavigationResult addRoute(const NXRouteConfig &config);
  NXRouterType::NavigationResult addRoutes(const QVector<NXRouteConfig> &configs);
  NXRouterType::NavigationResult addDynamicRoute(const QString &parentPath, const NXRouteConfig &config);
  NXRouterType::NavigationResult removeRoute(const QString &path);

  bool hasRoute(const QString &path) const;
  QStringList getRoutePaths() const;
  QVariantMap getRouteMeta(const QString &path) const;

  NXRouterType::NavigationResult push(const QString &path, const QVariantMap &params = {});
  NXRouterType::NavigationResult replace(const QString &path, const QVariantMap &params = {});
  void back();
  void forward();

  QString getCurrentPath() const;
  QVariantMap getCurrentParams() const;

  int beforeEach(const NXRouteGuard &guard);
  int afterEach(const NXRouteAfterHook &hook);
  void removeBeforeGuard(int guardId);
  void removeAfterHook(int hookId);

  void setRouteBeforeEnter(const QString &path, const NXRouteGuard &guard);

  void installRoutes();
  void resetRouter();
  Q_SIGNAL void routeChanged(const QString &path, const QVariantMap &params);
  Q_SIGNAL void navigationBlocked(const QString &path);
  Q_SIGNAL void routeTableChanged();
};

#pragma pop_macro("Q_DISABLE_COPY")
#define eRouter NXRouter::getInstance()

#endif // NXROUTER_H
