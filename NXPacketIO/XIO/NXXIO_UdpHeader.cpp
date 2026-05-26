#include "xio/NXXIO_UdpHeader.h"
#include <iostream>

#include "GenI.h"
#include "GenO.h"
NXXIO_UdpHeader::NXXIO_UdpHeader(int aVersionID)
    : mXIO_Version(0xfeedbeef ^ aVersionID)
{
    mReceivedInvalidPacket = false;
}

NXXIO_UdpHeader::NXXIO_UdpHeader(const NXXIO_UdpHeader& aSrc)
    : PakHeader(aSrc),
      mXIO_Version(aSrc.mXIO_Version),
      mReceivedInvalidPacket(aSrc.mReceivedInvalidPacket)
{
}

PakHeader* NXXIO_UdpHeader::Clone() const
{
    return new NXXIO_UdpHeader(*this);
}

void NXXIO_UdpHeader::WriteHeader(GenBuffer& aIO, int aPacketID, int aPacketLength)
{
    aIO.putValue(mXIO_Version);
    aIO.putValue(static_cast<int32_t>(aPacketLength - sizeof(header)));
    aIO.putValue(static_cast<int32_t>(aPacketID));
}

bool NXXIO_UdpHeader::ReadHeader(GenBuffer& aIO, int& aPacketID, int& aPacketLength)
{
    uint32_t version;
    int32_t length, id;
    aIO.getValue(version);
    aIO.getValue(length);
    aPacketLength = length + sizeof(header);
    aIO.getValue(id);
    aPacketID = id;
    bool valid = (version == mXIO_Version);
    if (!valid && !mReceivedInvalidPacket)
    {
        mReceivedInvalidPacket = true;
        std::cout << "xio_interface: Received PDU with invalid version." << "XIO versions may not match." << std::endl;
    }
    return valid;
}

int NXXIO_UdpHeader::GetHeaderSize()
{
    return sizeof(header);
}
