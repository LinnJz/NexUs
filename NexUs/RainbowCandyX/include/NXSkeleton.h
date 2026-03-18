#ifndef NXSKELETON_H
#define NXSKELETON_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXSkeletonPrivate;
class NX_EXPORT NXSkeleton : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXSkeleton)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(bool, IsAnimated)
public:
    enum SkeletonType
    {
        Text = 0,
        Circle,
        Rectangle
    };
    Q_ENUM(SkeletonType)

    explicit NXSkeleton(QWidget* parent = nullptr);
    ~NXSkeleton() override;

    void setSkeletonType(SkeletonType type) noexcept;
    SkeletonType getSkeletonType() const noexcept;

protected:
  void paintEvent(QPaintEvent* event) override;
  QSize sizeHint() const override;
};

#endif // NXSKELETON_H
