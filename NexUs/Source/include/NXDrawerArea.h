#ifndef NXFRAMEWORK_NEXUS_INCLUDE_NXDRAWERAREA_H_
#define NXFRAMEWORK_NEXUS_INCLUDE_NXDRAWERAREA_H_

#include <QWidget>
#include "NXProperty.h"

class NXDrawerAreaPrivate;

class NX_EXPORT NXDrawerArea : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXDrawerArea)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, HeaderHeight)

public:
  explicit NXDrawerArea(QWidget *parent = nullptr);
  ~NXDrawerArea() override;

  void setDrawerHeader(QWidget *widget) noexcept;

  void addDrawer(QWidget *widget) noexcept;
  void removeDrawer(QWidget *widget) noexcept;

  void expand() noexcept;
  void collapse() noexcept;

  bool getIsExpand() const noexcept;
Q_SIGNALS:
  void expandStateChanged(bool isExpand);
};

#endif // NXFRAMEWORK_NEXUS_INCLUDE_NXDRAWERAREA_H_
