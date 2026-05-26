#ifndef PakHeader_H
#define PakHeader_H

#include "NXPacketIO_Export.h"

#include "GenBuffer.h"

class NX_PACKETIO_EXPORT PakHeader
{
public:
    virtual PakHeader* Clone() const = 0;
    virtual ~PakHeader() {}

    virtual void WriteHeader(GenBuffer& aIO, int aPacketID, int aPacketLength) = 0;
    virtual bool ReadHeader(GenBuffer& aIO, int& aPacketID, int& aPacketLength) = 0;
    virtual int GetHeaderSize() = 0;
};

#endif
