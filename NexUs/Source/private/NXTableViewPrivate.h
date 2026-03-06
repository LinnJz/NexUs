#ifndef NXTABLEVIEWPRIVATE_H
#define NXTABLEVIEWPRIVATE_H

#include <QMetaObject>
#include <QObject>

#include "NXProperty.h"

class NXTableView;
class NXTableViewStyle;

class NXTableViewPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTableView)

public:
  explicit NXTableViewPrivate(QObject *parent = nullptr);
  ~NXTableViewPrivate();

private:
  NXTableViewStyle *_tableViewStyle { nullptr };
  QMetaObject::Connection _modelResetConnection;
  QMetaObject::Connection _columnsInsertedConnection;
  QMetaObject::Connection _columnsRemovedConnection;
  QMetaObject::Connection _layoutChangedConnection;
};

#endif // NXTABLEVIEWPRIVATE_H
