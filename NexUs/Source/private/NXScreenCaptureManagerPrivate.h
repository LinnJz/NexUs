#ifndef NXSCREENCAPTUREMANAGERPRIVATE_H
#define NXSCREENCAPTUREMANAGERPRIVATE_H
#include <QObject>
#ifdef Q_OS_MAC
#include <QImage>
#include "NXProperty.h"
class NXScreenCapture;
class NXScreenCaptureManager;

class NXScreenCaptureManagerPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXScreenCaptureManager)
public:
  explicit NXScreenCaptureManagerPrivate(QObject *parent = nullptr);
  ~NXScreenCaptureManagerPrivate();

private:
  Q_SIGNAL void grabScreen();
  bool _isAllowedGrabScreen { false };
  NXScreenCapture *_screenCapture { nullptr };
  QThread *_captureThread { nullptr };
};

class NXScreenCaptureScreen;

class NXScreenCaptureScreenPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXScreenCaptureScreen)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
  explicit NXScreenCaptureScreenPrivate(QObject *parent = nullptr);
  ~NXScreenCaptureScreenPrivate();

private:
  NXScreenCaptureManager *_captureManager { nullptr };
  bool _isSyncGrabSize { false };
  QImage _img;
};
#endif
#endif // NXSCREENCAPTUREMANAGERPRIVATE_H
