#ifndef NXSPLITTERPRIVATE_H
#define NXSPLITTERPRIVATE_H

#include <QObject>

#include "NXDef.h"

class NXSplitter;

class NXSplitterPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSplitter)
  Q_PROPERTY_CREATE_D(int, HandleWidth)
  Q_PROPERTY_CREATE_D(int, GripLength)

public:
  explicit NXSplitterPrivate(QObject *parent = nullptr);
  ~NXSplitterPrivate();
};

#endif // NXSPLITTERPRIVATE_H
