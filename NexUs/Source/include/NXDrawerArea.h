#ifndef NXFRAMEWORK_NEXUS_INCLUDE_NXDRAWERAREA_H_
#define NXFRAMEWORK_NEXUS_INCLUDE_NXDRAWERAREA_H_

#include <QWidget>
#include "NXProperty.h"

class NXDrawerAreaPrivate;

class NX_EXPORT NXDrawerArea : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXDrawerArea)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, HeaderHeight)
public:
  explicit NXDrawerArea(QWidget *parent = nullptr);
  ~NXDrawerArea() override;

  void setDrawerHeader(QWidget *widget);

  void addDrawer(QWidget *widget);
  void removeDrawer(QWidget *widget);

  void expand();
  void collapse();

  bool getIsExpand() const;
Q_SIGNALS:
  Q_SIGNAL void expandStateChanged(bool isExpand);
};

#endif // NXFRAMEWORK_NEXUS_INCLUDE_NXDRAWERAREA_H_
