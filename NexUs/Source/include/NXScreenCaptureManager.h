#ifndef NXSCREENCAPTUREMANAGER_H
#define NXSCREENCAPTUREMANAGER_H

#include <QWidget>
#ifdef Q_OS_MAC
#include "NXProperty.h"
#include "NXSingleton.h"

class NXScreenCaptureManagerPrivate;

class NX_EXPORT NXScreenCaptureManager : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXScreenCaptureManager)
  LINN_SINGLETON_CREATE(LINN_SINGLETON_UNIQUE(NXScreenCaptureManager))

private:
  explicit NXScreenCaptureManager(QObject *parent = nullptr);
  ~NXScreenCaptureManager() override;

public:
  QStringList getDisplayList() const;
  QImage grabScreenToImage() const;
  void startGrabScreen();
  void stopGrabScreen();
  bool getIsGrabScreen() const;
  bool setDisplayID(int displayID);
  int getDisplayID() const;
  void setGrabArea(int width, int height);
  void setGrabArea(int x, int y, int width, int height);
  QRect getGrabArea() const;
  void setGrabFrameRate(int frameRateValue);
  int getGrabFrameRate() const;
Q_SIGNALS:
  Q_SIGNAL void grabImageUpdate(QImage img);
};

class NXScreenCaptureScreenPrivate;

class NX_EXPORT NXScreenCaptureScreen : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXScreenCaptureScreen)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
  explicit NXScreenCaptureScreen(QWidget *parent = nullptr);
  ~NXScreenCaptureScreen();
  void setIsSyncGrabSize(bool isSyncGrabSize);
  bool getIsSyncGrabSize() const;

protected:
  void paintEvent(QPaintEvent *event) override;
};
#endif
#endif // NXSCREENCAPTUREMANAGER_H
