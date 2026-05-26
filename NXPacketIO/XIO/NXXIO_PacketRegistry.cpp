#include "NXXIO_PacketRegistry.h"

#include <cassert>

#include "PakProcessor.h"
#include "PakSerializeImpl.h"
#include "PakTypeDictionary.h"

int NXXIO_PacketRegistry::sPacketVersion = 0;

#define VALID_ID_RANGE(ID) ((ID) >= 1)
#define REGISTER_PACKET(Z)                  \
    aProcessor.RegisterPacket(#Z, new (Z)); \
    assert(VALID_ID_RANGE(Z::cPACKET_ID));

void NXXIO_PacketRegistry::registerPackets(PakProcessor& aProcessor)
{
    registerClasses();
    REGISTER_PACKET(NXXIO_HeartbeatPkt);
    REGISTER_PACKET(NXXIO_InitializePkt);
    REGISTER_PACKET(NXXIO_ExamplePkt);
    REGISTER_PACKET(NXXIO_ScreenPkt);
}

void NXXIO_PacketRegistry::registerClasses()
{
    static bool isRegistered(false);
    if (!isRegistered)
    {
        isRegistered = true;
        int typeId = 0;
        //        PakTypeDictionary::GetInstance().RegisterType<WsfTaskResource>(++typeId);
        //        PakTypeDictionary::GetInstance().RegisterType<WsfXIO_ExtraInfo>(++typeId);
        //        PakTypeDictionary::GetInstance().RegisterType<WsfXIO_PlatformPartInfo>(++typeId);
        //        PakTypeDictionary::GetInstance().RegisterType<WsfXIO_CommInfo>(++typeId);
        //        PakTypeDictionary::GetInstance().RegisterType<WsfXIO_ProcessorInfo>(++typeId);
        //        PakTypeDictionary::GetInstance().RegisterType<WsfXIO_SensorInfo>(++typeId);
        //        assert(cXIO_WSF_LAST_TYPE_ID >= typeId);
    }
}
