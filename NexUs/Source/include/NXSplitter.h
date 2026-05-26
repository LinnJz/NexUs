#ifndef NXSPLITTER_H
#define NXSPLITTER_H

#include <QSplitter>

#include "NXProperty.h"

class NXSplitterPrivate;

class NX_EXPORT NXSplitter : public QSplitter
{
  Q_OBJECT
  Q_Q_CREATE(NXSplitter)
  Q_PROPERTY_CREATE_H(int, HandleWidth)
  Q_PROPERTY_CREATE_H(int, GripLength)

public:
  explicit NXSplitter(QWidget *parent = nullptr);
  explicit NXSplitter(Qt::Orientation orientation, QWidget *parent = nullptr);
  ~NXSplitter();

protected:
  QSplitterHandle *createHandle() override;
};

#endif // NXSPLITTER_H
