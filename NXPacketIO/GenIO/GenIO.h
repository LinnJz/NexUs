#ifndef GENIO_H
#define GENIO_H

#include "NXPacketIO_Export.h"

#include "GenI.h"
#include "GenO.h"

class NX_PACKETIO_EXPORT GenIO : public GenI, public GenO
{
public:
    GenIO(const char* aRecvBufName, const char* aSendBufName);
    ~GenIO() override;

private:
};

#endif
