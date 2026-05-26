#ifndef NXNAVIGATIONROUTERPRIVATE_H
#define NXNAVIGATIONROUTERPRIVATE_H
#include <QMap>
#include <QObject>
#include <QVariantMap>
#include <QVector>

#include "NXProperty.h"
class NXNavigationRouter;

class NXNavigationRouterPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXNavigationRouter)
  Q_PROPERTY_CREATE_D(int, MaxRouteCount)

public:
  explicit NXNavigationRouterPrivate(QObject *parent = nullptr);
  ~NXNavigationRouterPrivate();

private:
  struct RouteContext
  {
    int currentIndex { -1 };
    QVector<QVariantMap> routeList;
  };

  int _currentIndex { -1 };
  QMap<QObject *, RouteContext> _contextMap;
  QVector<QVariantMap> _routeList;
};

#endif // NXNAVIGATIONROUTERPRIVATE_H
