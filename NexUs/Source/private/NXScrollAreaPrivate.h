#ifndef NXSCROLLAREAPRIVATE_H
#define NXSCROLLAREAPRIVATE_H

#include <QObject>
#include <QScrollBar>

#include "NXProperty.h"
class NXScrollBar;
class NXScrollArea;

class NXScrollAreaPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXScrollArea)
public:
  explicit NXScrollAreaPrivate(QObject *parent = nullptr);
  ~NXScrollAreaPrivate();
};

#endif // NXSCROLLAREAPRIVATE_H
