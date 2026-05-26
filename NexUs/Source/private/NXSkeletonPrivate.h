#ifndef NXSKELETONPRIVATE_H
#define NXSKELETONPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXSkeleton.h"
class QVariantAnimation;

class NXSkeletonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSkeleton)
  Q_PROPERTY_CREATE(qreal, ShimmerPosition)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(bool, IsAnimated)

public:
  explicit NXSkeletonPrivate(QObject *parent = nullptr);
  ~NXSkeletonPrivate();

private:
  NXSkeleton::SkeletonType _skeletonType { NXSkeleton::Text };
  NXThemeType::ThemeMode _themeMode;
  QVariantAnimation *_shimmerAnimation { nullptr };
  void _startShimmerAnimation();
  void _stopShimmerAnimation();
};

#endif // NXSKELETONPRIVATE_H
