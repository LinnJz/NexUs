#ifndef NXNAVIGATIONROUTERPRIVATE_H
#define NXNAVIGATIONROUTERPRIVATE_H
#include <QList>
#include <QObject>
#include <QVariantMap>

#include "NXProperty.h"
class NXNavigationRouter;

class NXNavigationRouterPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXNavigationRouter)
  Q_PROPERTY_CREATE_D(int, MaxRouteCount)
public:
  explicit NXNavigationRouterPrivate(QObject *parent = nullptr);
  ~NXNavigationRouterPrivate() override;

private:
  int _currentIndex { -1 };
  QList<QVariantMap> _routeList;
};

#endif // NXNAVIGATIONROUTERPRIVATE_H
