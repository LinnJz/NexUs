#ifndef NXBREADCRUMBBAR_H
#define NXBREADCRUMBBAR_H
#include <QWidget>

#include "NXProperty.h"

class NXBreadcrumbBarPrivate;

class NX_EXPORT NXBreadcrumbBar : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXBreadcrumbBar)
  Q_PROPERTY_CREATE_Q_H(int, TextPixelSize)
  Q_PROPERTY_CREATE_Q_H(bool, IsAutoRemove)
public:
  explicit NXBreadcrumbBar(QWidget *parent = nullptr);
  ~NXBreadcrumbBar();
  void setBreadcrumbList(QStringList breadcrumbList);
  QStringList appendBreadcrumb(const QString& breadcrumb);
  QStringList removeBreadcrumb(const QString& breadcrumb);

  int getBreadcrumbListCount() const;
  QStringList getBreadcrumbList() const;

Q_SIGNALS:
  Q_SIGNAL void breadcrumbClicked(const QString& breadcrumb, QStringList lastBreadcrumbList);
};

#endif // NXBREADCRUMBBAR_H
