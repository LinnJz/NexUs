#ifndef NXXIO_PACKET_H
#define NXXIO_PACKET_H

#include "NXPacketIO_Export.h"

#include "GenUniqueId.h"
#include "PakPacket.h"

class NX_PACKETIO_EXPORT NXXIO_Packet : public PakPacket
{
public:
    typedef bool BaseType;
    static const int cPACKET_ID = -1;

    NXXIO_Packet(int packetID)
        : PakPacket(packetID)
    {
    }

    template <typename T>
    void Serialize(T& serializeBuf)
    {
        serializeBuf & _applicationId & _baseTime;
    }
    double getBaseTime() const { return _baseTime; }
    void SetBaseTime(double baseTime) { _baseTime = baseTime; }
    GenUniqueId _applicationId;

protected:
    double _baseTime;
};

#define XIO_DEFINE_PACKET_CTOR(PACKET_NAME__, BASE_TYPE__, PACKET_ID__) \
    typedef BASE_TYPE__ BaseType;                                       \
    static const int cPACKET_ID = PACKET_ID__;                          \
    PACKET_NAME__(int aPacketId = cPACKET_ID)                           \
        : BASE_TYPE__(aPacketId)

#define XIO_DEFINE_PACKET_SERIALIZE()      \
    template <typename T>                  \
    void Serialize(T& serializeBuf)        \
    {                                      \
        BaseType::Serialize(serializeBuf); \
        DoSerialize(serializeBuf);         \
    }                                      \
    template <typename T>                  \
    void DoSerialize(T& serializeBuf)

#define XIO_DEFINE_PACKET(PACKET_NAME__, BASE_TYPE__, PACKET_ID__)     \
    XIO_DEFINE_PACKET_CTOR(PACKET_NAME__, BASE_TYPE__, PACKET_ID__) {} \
    XIO_DEFINE_PACKET_SERIALIZE()

#define XIO_DEFINE_STRUCT           \
    template <typename T>           \
    void Serialize(T& serializeBuf) \
    {                               \
        DoSerialize(serializeBuf);  \
    }                               \
    template <typename T>           \
    void DoSerialize(T& serializeBuf)

#define XIO_DEFINE_CLEANUP \
    virtual void ReceiveCleanup() override

#endif
