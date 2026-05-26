#ifndef GENERROR_H
#define GENERROR_H

#include "NXPacketIO_Export.h"

class NX_PACKETIO_EXPORT GenError
{
public:
    enum GenErrorMask
    {
        cIO_ERRORS = 0x01,
        cCONNECT_ERRORS = 0x02
    };

    static void SetSuppressErrorMask(unsigned int aSuppressErrorMask);

    static unsigned int GetSuppressErrorMask();

    static bool ShowError(unsigned int aErrorMask);

private:
    static unsigned int mSuppressErrorMask;
};
#endif
