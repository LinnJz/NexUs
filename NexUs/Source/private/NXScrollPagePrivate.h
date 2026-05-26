#ifndef NXSCROLLPAGEPRIVATE_H
#define NXSCROLLPAGEPRIVATE_H

#include <QMap>
#include <QObject>

#include "NXProperty.h"
class NXScrollPage;
class NXScrollArea;
class QHBoxLayout;
class QVBoxLayout;
class NXText;
class QStackedWidget;
class NXBreadcrumbBar;

class NXScrollPagePrivate : public QObject
{
  friend class NXScrollPageRouteCommand;
  Q_OBJECT
  Q_D_CREATE(NXScrollPage)
  Q_PROPERTY_CREATE_D(QWidget *, CustomWidget)

public:
  explicit NXScrollPagePrivate(QObject *parent = nullptr);
  ~NXScrollPagePrivate();
  Q_INVOKABLE void onNavigationRoute(const QVariantMap &routeData);

private:
  bool _isGrabGesture { false };
  int _navigationTargetIndex { 0 };
  int _pageTitleSpacing { 0 };
  QMap<QString, int> _centralWidgetMap;
  QHBoxLayout *_pageTitleLayout { nullptr };
  QVBoxLayout *_mainLayout { nullptr };
  QStackedWidget *_centralStackedWidget { nullptr };
  NXBreadcrumbBar *_breadcrumbBar { nullptr };
  void _switchCentralStackIndex(int targetIndex, int lastIndex);
};

#endif // NXSCROLLPAGEPRIVATE_H
