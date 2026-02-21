#ifndef NXEXPONENTIALBLURPRIVATE_H
#define NXEXPONENTIALBLURPRIVATE_H

#include <QObject>

#include "NXProperty.h"

class NXExponentialBlur;

class NXExponentialBlurPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXExponentialBlur)
public:
  explicit NXExponentialBlurPrivate(QObject *parent = nullptr);
  ~NXExponentialBlurPrivate();

private:
  static int _aprec;
  static int _zprec;
  static void _drawExponentialBlur(QImage& image, const quint16& qRadius);
  static void _drawRowBlur(QImage& image, const int& row, const int& alpha);
  static void _drawColumnBlur(QImage& image, const int& column, const int& alpha);
  static void _drawInnerBlur(unsigned char *bptr, int& zR, int& zG, int& zB, int& zA, int alpha);
};

#endif // NXEXPONENTIALBLURPRIVATE_H
