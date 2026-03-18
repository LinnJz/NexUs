#ifndef NXBREADCRUMBBAR_H
#define NXBREADCRUMBBAR_H
#include <QWidget>

#include "NXProperty.h"

class NXBreadcrumbBarPrivate;

class NX_EXPORT NXBreadcrumbBar : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXBreadcrumbBar)
  Q_PROPERTY_CREATE_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_H(bool, IsAutoRemove)

public:
  explicit NXBreadcrumbBar(QWidget *parent = nullptr);
  ~NXBreadcrumbBar();
  void setBreadcrumbList(const QStringList& breadcrumbList) noexcept;
  QStringList appendBreadcrumb(const QString& breadcrumb) noexcept;
  QStringList removeBreadcrumb(const QString& breadcrumb) noexcept;

  int getBreadcrumbListCount() const noexcept;
  QStringList getBreadcrumbList() const noexcept;

Q_SIGNALS:
  void breadcrumbClicked(const QString& breadcrumb, const QStringList& lastBreadcrumbList);
};

#endif // NXBREADCRUMBBAR_H
