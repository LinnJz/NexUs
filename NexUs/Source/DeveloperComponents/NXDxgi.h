#ifndef NXDXGI_H
#define NXDXGI_H

#include <QObject>
#ifdef Q_OS_WIN
#  include <QElapsedTimer>
#  include <QPixmap>
#  include <d3d11.h>
#  include <dxgi1_6.h>

#  include "NXProperty.h"

class NXDxgi : public QObject
{
  Q_OBJECT
  Q_PRIVATE_CREATE(QRect, GrabArea)
  Q_PRIVATE_CREATE(QS_SET_CREF(QStringList), DxDeviceList)
  Q_PRIVATE_CREATE(QS_SET_CREF(QStringList), OutputDeviceList)
  Q_PRIVATE_CREATE(QS_SET_CREF(QString), LastError)
  Q_PRIVATE_CREATE(int, DxDeviceID)
  Q_PRIVATE_CREATE(int, OutputDeviceID)
  Q_PRIVATE_CREATE(int, GrabFrameRate)
  Q_PRIVATE_CREATE(int, TimeoutMsValue)
  Q_PRIVATE_CREATE(bool, IsGrabActive)
  Q_PRIVATE_CREATE(bool, IsInitSuccess)
  Q_PRIVATE_CREATE(bool, IsGrabStoped)
  Q_PRIVATE_CREATE(bool, IsGrabCenter)

public:
  explicit NXDxgi(QObject *parent = nullptr);
  ~NXDxgi();
  bool initialize(int dxID, int outputID);
  QImage getGrabImage() const;
  Q_SLOT void onGrabScreen();
  Q_SIGNAL void grabScreenOver(const QImage &img);

private:
  int _descWidth { 0 };
  int _descHeight { 0 };
  qint64 _lastGrabTime { 0 };
  qint64 _cpuSleepTime { 0 };
  QElapsedTimer _grabTimer;
  IDXGIOutputDuplication *_duplication { nullptr };
  ID3D11Device *_device { nullptr };
  ID3D11DeviceContext *_context { nullptr };
  ID3D11Texture2D *_texture { nullptr };
  uchar *_imageBits { nullptr };
  void releaseInterface();
  void cpuSleep(qint64 usec);
};
#endif
#endif // NXDXGI_H
