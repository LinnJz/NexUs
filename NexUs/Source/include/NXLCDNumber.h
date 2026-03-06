#ifndef NXWIDGETTOOLS_NXLCDNUMBER_H
#define NXWIDGETTOOLS_NXLCDNUMBER_H

#include <QLCDNumber>

#include "NXProperty.h"

class NXLCDNumberPrivate;

class NX_EXPORT NXLCDNumber : public QLCDNumber
{
  Q_OBJECT
  Q_Q_CREATE(NXLCDNumber)
  Q_PROPERTY_CREATE_Q_H(bool, IsUseAutoClock)
  Q_PROPERTY_CREATE_Q_EX_H(const QString&, QString, AutoClockFormat)
  Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)

public:
  explicit NXLCDNumber(QWidget *parent = nullptr);
  explicit NXLCDNumber(uint numDigits, QWidget *parent = nullptr);
  ~NXLCDNumber() override;

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXWIDGETTOOLS_NXLCDNUMBER_H
