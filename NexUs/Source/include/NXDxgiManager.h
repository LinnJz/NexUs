#ifndef NXDXGIMANAGER_H
#define NXDXGIMANAGER_H

#include <QWidget>
#ifdef Q_OS_WIN
#  include "LinnSingleton.h"
#  include "NXProperty.h"

#  pragma push_macro("Q_DISABLE_COPY")
#  undef Q_DISABLE_COPY
#  define Q_DISABLE_COPY(Class)

class NXDxgiManagerPrivate;

class NX_EXPORT NXDxgiManager : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXDxgiManager)
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXDxgiManager))

private:
  explicit NXDxgiManager(QObject *parent = nullptr);
  ~NXDxgiManager();

public:
  QStringList getDxDeviceList() const noexcept;
  QStringList getOutputDeviceList() const noexcept;
  QImage grabScreenToImage() const noexcept;
  void startGrabScreen() noexcept;
  void stopGrabScreen() noexcept;
  bool getIsGrabScreen() const noexcept;
  bool setDxDeviceID(int dxID) noexcept;
  int getDxDeviceID() const noexcept;
  bool setOutputDeviceID(int deviceID) noexcept;
  int getOutputDeviceID() const noexcept;
  void setGrabArea(int width, int height) noexcept; // 从屏幕中心向外延伸
  void setGrabArea(int x, int y, int width, int height) noexcept;
  QRect getGrabArea() const noexcept;
  void setGrabFrameRate(int frameRateValue) noexcept;
  int getGrabFrameRate() const noexcept;
  void setTimeoutMsValue(int timeoutValue) noexcept;
  int getTimeoutMsValue() const noexcept;
Q_SIGNALS:
  void grabImageUpdate(const QImage& img);
};

#  pragma pop_macro("Q_DISABLE_COPY")

class NXDxgiScreenPrivate;

class NX_EXPORT NXDxgiScreen : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXDxgiScreen)
  Q_PROPERTY_CREATE_H(int, BorderRadius)

public:
  explicit NXDxgiScreen(QWidget *parent = nullptr);
  ~NXDxgiScreen();
  void setIsSyncGrabSize(bool isSyncGrabSize) noexcept;
  bool getIsSyncGrabSize() const noexcept;

protected:
  void paintEvent(QPaintEvent *event) override;
};
#endif
#endif // NXDXGIMANAGER_H
