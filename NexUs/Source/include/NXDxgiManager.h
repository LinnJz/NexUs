#ifndef NXDXGIMANAGER_H
#define NXDXGIMANAGER_H

#include <QWidget>
#ifdef Q_OS_WIN
#  include "NXProperty.h"
#  include "singleton.h"

class NXDxgiManagerPrivate;

class NX_EXPORT NXDxgiManager : public QObject
{
#  pragma push_macro("DISABLE_COPY")
#  undef DISABLE_COPY
#  define DISABLE_COPY(Class)
  SINGLETON_CREATE_H(NXDxgiManager)
#  pragma pop_macro("DISABLE_COPY")
  Q_OBJECT
  Q_Q_CREATE(NXDxgiManager)

private:
  explicit NXDxgiManager(QObject *parent = nullptr);
  ~NXDxgiManager();

public:
  QStringList getDxDeviceList() const;
  QStringList getOutputDeviceList() const;
  QImage grabScreenToImage() const;
  void startGrabScreen();
  void stopGrabScreen();
  bool getIsGrabScreen() const;
  bool setDxDeviceID(int dxID);
  int getDxDeviceID() const;
  bool setOutputDeviceID(int deviceID);
  int getOutputDeviceID() const;
  void setGrabArea(int width, int height); // 从屏幕中心向外延伸
  void setGrabArea(int x, int y, int width, int height);
  QRect getGrabArea() const;
  void setGrabFrameRate(int frameRateValue);
  int getGrabFrameRate() const;
  void setTimeoutMsValue(int timeoutValue);
  int getTimeoutMsValue() const;
Q_SIGNALS:
  Q_SIGNAL void grabImageUpdate(QImage img);
};

class NXDxgiScreenPrivate;

class NX_EXPORT NXDxgiScreen : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXDxgiScreen)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)

public:
  explicit NXDxgiScreen(QWidget *parent = nullptr);
  ~NXDxgiScreen();
  void setIsSyncGrabSize(bool isSyncGrabSize);
  bool getIsSyncGrabSize() const;

protected:
  void paintEvent(QPaintEvent *event) override;
};
#endif
#endif // NXDXGIMANAGER_H
