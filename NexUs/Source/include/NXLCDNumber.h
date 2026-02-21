#ifndef NEXUS_NXLCDNUMBER_H
#define NEXUS_NXLCDNUMBER_H

#include <QLCDNumber>

#include "NXProperty.h"

class NXLCDNumberPrivate;

class NX_EXPORT NXLCDNumber : public QLCDNumber
{
  Q_OBJECT
  Q_Q_CREATE(NXLCDNumber)
  Q_PROPERTY_CREATE_Q_H(bool, IsUseAutoClock)
  Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)

public:
  explicit NXLCDNumber(QWidget *parent = nullptr);
  explicit NXLCDNumber(uint numDigits, QWidget *parent = nullptr);
  ~NXLCDNumber() override;

  void setAutoClockFormat(const QString& format);
  QString getAutoClockFormat() const;

  Q_SIGNAL void pAutoClockFormatChanged();

protected:
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NEXUS_NXLCDNUMBER_H
