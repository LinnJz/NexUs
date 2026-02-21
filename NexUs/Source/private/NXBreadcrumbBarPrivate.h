#ifndef NXBREADCRUMBBARPRIVATE_H
#define NXBREADCRUMBBARPRIVATE_H

#include <QObject>

#include "NXProperty.h"
class NXBaseListView;
class NXBreadcrumbBarModel;
class NXBreadcrumbBarDelegate;

class NXBreadcrumbBar;

class NXBreadcrumbBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXBreadcrumbBar)
  Q_PROPERTY_CREATE_D(int, TextPixelSize)
  Q_PROPERTY_CREATE_D(bool, IsAutoRemove)
public:
  explicit NXBreadcrumbBarPrivate(QObject *parent = nullptr);
  ~NXBreadcrumbBarPrivate();

private:
  NXBreadcrumbBarModel *_listModel { nullptr };
  NXBreadcrumbBarDelegate *_listDelegate { nullptr };
  NXBaseListView *_listView { nullptr };
};

#endif // NXBREADCRUMBBARPRIVATE_H
