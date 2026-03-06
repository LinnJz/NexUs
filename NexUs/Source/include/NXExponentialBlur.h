#ifndef NXEXPONENTIALBLUR_H
#define NXEXPONENTIALBLUR_H

#include <QObject>

#include "LinnSingleton.h"
#include "NXProperty.h"

#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(Class)

class NXExponentialBlurPrivate;

class NX_EXPORT NXExponentialBlur : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXExponentialBlur)
  LINN_SINGLETON_CREATE(LINN_SINGLETON_UNIQUE(NXExponentialBlur))

private:
  explicit NXExponentialBlur(QObject *parent = nullptr);
  ~NXExponentialBlur();

public:
  static QPixmap doExponentialBlur(QImage img, const quint16& blurRadius);
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXEXPONENTIALBLUR_H
