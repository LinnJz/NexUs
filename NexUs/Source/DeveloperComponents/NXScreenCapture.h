#ifndef NXSCREENCAPTURE_H
#define NXSCREENCAPTURE_H

#include <QObject>
#ifdef Q_OS_MAC
#include <QElapsedTimer>
#include <QImage>

#include "NXProperty.h"

class NXScreenCapture : public QObject
{
  Q_OBJECT
  Q_PRIVATE_CREATE(QStringList, DisplayList)
  Q_PRIVATE_CREATE(int, DisplayID)
  Q_PRIVATE_CREATE(QString, LastError)
  Q_PRIVATE_CREATE(bool, IsGrabActive)
  Q_PRIVATE_CREATE(QRect, GrabArea)
  Q_PRIVATE_CREATE(int, GrabFrameRate)
  Q_PRIVATE_CREATE(bool, IsInitSuccess)
  Q_PRIVATE_CREATE(bool, IsGrabStoped)
  Q_PRIVATE_CREATE(bool, IsGrabCenter)

public:
  explicit NXScreenCapture(QObject *parent = nullptr);
  ~NXScreenCapture() override;
  bool initialize(int displayID);
  QImage getGrabImage() const;
  Q_SLOT void onGrabScreen();
  Q_SIGNAL void grabScreenOver(QImage img);

private:
  class Private;
  Private *d { nullptr };
  QElapsedTimer _grabTimer;
  qint64 _lastGrabTime { 0 };
  qint64 _cpuSleepTime { 0 };
  QImage _lastImage;
  int _displayWidth { 0 };
  int _displayHeight { 0 };
  void releaseInterface();
  void cpuSleep(qint64 usec);
};
#endif
#endif // NXSCREENCAPTURE_H
