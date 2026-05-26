#ifndef NXXIO_UDPHEADER_H
#define NXXIO_UDPHEADER_H

#include "NXPacketIO_Export.h"

#include "PakHeader.h"
#include "PakIntTypes.h"

class NX_PACKETIO_EXPORT NXXIO_UdpHeader : public PakHeader
{
public:
    explicit NXXIO_UdpHeader(int aVersionID);
    NXXIO_UdpHeader(const NXXIO_UdpHeader& aSrc);
    ~NXXIO_UdpHeader() override = default;

    PakHeader* Clone() const override;

    void WriteHeader(GenBuffer& aIO, int aPacketID, int aPacketLength) override;

    bool ReadHeader(GenBuffer& aIO, int& aPacketID, int& aPacketLength) override;

    int GetHeaderSize() override;

protected:
    struct header {
        uint32_t mXIO_Version;
        int32_t mLength;
        int32_t mID;
    };
    uint32_t mXIO_Version;
    bool mReceivedInvalidPacket;
};

#endif
