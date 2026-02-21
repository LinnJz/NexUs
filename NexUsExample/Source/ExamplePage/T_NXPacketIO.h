#ifndef NXFRAMEWORK_NXWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_NXPACKETIO_H_
#define NXFRAMEWORK_NXWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_NXPACKETIO_H_

#include <QObject>
#if defined(Q_OS_WIN) && defined(BUILD_WITH_NXPACKETIO)
#include <QImage>
#include <QMutex>
#include <QQueue>
#include "NXProperty.h"
#include "NXXIO_PacketRegistry.h"
#include "UtCallbackHolder.h"
class NXXIO_Interface;
class PakPacket;
class NXXIO_Connection;

class T_NXPacketIO : public QObject
{
  Q_OBJECT
  Q_PRIVATE_CREATE(bool, IsActive);
  Q_PRIVATE_CREATE(QString, InterfaceIP);

public:
  Q_INVOKABLE explicit T_NXPacketIO(QObject *parent = nullptr);
  ~T_NXPacketIO() override;

  Q_SLOT void handleGrabImage();
  Q_SLOT void handleImagePacket();

  Q_SIGNAL void sendHandleResult(QPixmap map);

private:
  QMutex _mutex;
  int _lastImageIndex { 0 };
  QByteArray _imageArray;
  std::unique_ptr<NXXIO_Interface> _interface;
  UtCallbackHolder _callback;
  void _handleScreenPkt(NXXIO_ScreenPkt& screenPkt);
  NXXIO_Connection *_connection { nullptr };
  NXXIO_Connection *_multicastConnection { nullptr };
  void _sendToXIO(NXXIO_Packet& packet, bool isMulticast = false);
};
#endif
#endif // NXFRAMEWORK_NXWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_NXPACKETIO_H_
