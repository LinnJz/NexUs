#ifndef NXPAGINATION_H
#define NXPAGINATION_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXPaginationPrivate;
class NX_EXPORT NXPagination : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXPagination)
    Q_PROPERTY_CREATE_H(int, CurrentPage)
    Q_PROPERTY_CREATE_H(int, TotalPages)
    Q_PROPERTY_CREATE_H(int, ButtonSize)
    Q_PROPERTY_CREATE_H(int, PagerCount)
    Q_PROPERTY_CREATE_H(bool, JumperVisible)

public:
    explicit NXPagination(QWidget* parent = nullptr);
    ~NXPagination() override;

  Q_SIGNALS:
    void currentPageChanged(int page);

protected:
  void paintEvent(QPaintEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  QSize sizeHint() const override;
};

#endif // NXPAGINATION_H
