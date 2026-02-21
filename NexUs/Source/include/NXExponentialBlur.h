#ifndef NXEXPONENTIALBLUR_H
#define NXEXPONENTIALBLUR_H

#include <QObject>

#include "NXProperty.h"
#include "singleton.h"

class NXExponentialBlurPrivate;

class NX_EXPORT NXExponentialBlur : public QObject
{
#pragma push_macro("DISABLE_COPY")
#undef DISABLE_COPY
#define DISABLE_COPY(Class)
  SINGLETON_CREATE_H(NXExponentialBlur)
#pragma pop_macro("DISABLE_COPY")
  Q_OBJECT
  Q_Q_CREATE(NXExponentialBlur)

private:
  explicit NXExponentialBlur(QObject *parent = nullptr);
  ~NXExponentialBlur();

public:
  static QPixmap doExponentialBlur(QImage img, const quint16& blurRadius);
};

#endif // NXEXPONENTIALBLUR_H
