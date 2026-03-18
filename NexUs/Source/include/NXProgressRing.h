#ifndef NXFRAMEWORK_NEXUS_INCLUDE_NXPROGRESSRING_H_
#define NXFRAMEWORK_NEXUS_INCLUDE_NXPROGRESSRING_H_

#include "NXDef.h"

#include <QWidget>

class NXProgressRingPrivate;

class NX_EXPORT NXProgressRing : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXProgressRing);
  Q_PROPERTY_CREATE_H(bool, IsBusying)
  Q_PROPERTY_CREATE_H(bool, IsTransparent)
  Q_PROPERTY_CREATE_H(bool, IsDisplayValue)
  Q_PROPERTY_CREATE_H(int, BusyingWidth)
  Q_PROPERTY_CREATE_H(int, BusyingDurationTime)
  Q_PROPERTY_CREATE_H(int, Minimum)
  Q_PROPERTY_CREATE_H(int, Maximum)
  Q_PROPERTY_CREATE_H(int, Value)
  Q_PROPERTY_CREATE_H(int, ValuePixelSize)
  Q_PROPERTY_CREATE_H(NXProgressRingType::ValueDisplayMode, ValueDisplayMode)

public:
  explicit NXProgressRing(QWidget *parent = nullptr);
  ~NXProgressRing() override;

  void setRange(int min, int max) noexcept;

Q_SIGNALS:
  void rangeChanged(int min, int max);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXFRAMEWORK_NEXUS_INCLUDE_NXPROGRESSRING_H_
