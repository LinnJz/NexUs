#ifndef NXXIO_CONNECTION_H
#define NXXIO_CONNECTION_H

#include "NXPacketIO_Export.h"

#include <memory>
#include <string>

#include "GenUniqueId.h"
#include "PakConnection.h"
class PakSocketIO;
class PakTCP_IO;
class PakUDP_IO;
class NXXIO_Packet;
class NXXIO_Interface;

class NX_PACKETIO_EXPORT NXXIO_Connection : public PakConnection
{
public:
    friend class NXXIO_Interface;

    // 返回指向链接连接的指针（如果可用） 这仅对TCP连接有效，并返回不可靠的连接 能够与远程应用程序通信
    NXXIO_Connection* GetLinkedConnection() const { return mLinkedConnectionPtr; }
    void SetLinkedConnection(NXXIO_Connection* aConnectionPtr) { mLinkedConnectionPtr = aConnectionPtr; }

    PakSocketIO& GetIO() const { return *mIOPtr; }
    PakTCP_IO* GetTCP_IO() const { return mTCP_IO_Ptr; }
    PakUDP_IO* GetUDP_IO() const { return mUDP_IO_Ptr; }

    //! Returns the associated NXXIO_Interface
    NXXIO_Interface& getInterface() { return *mInterfacePtr; }

    //! Sends a packet to this connection
    void send(NXXIO_Packet& aPkt);

    //! Returns a locally unique identifier for this connection
    int getConnectionId() const { return mConnectionId; }

    //! Returns the name of the connected application
    const std::string& getApplicationName() const { return mApplicationName; }

    //! Sets the name of the connected application
    void setApplicationName(const std::string& aApplicationName) { mApplicationName = aApplicationName; }

    //! Sets the application ID
    void setApplicationId(GenUniqueId& aApplicationId) { mApplicationId = aApplicationId; }

    //! Returns the remote application's unique ID
    //! This is only valid for TCP connections (UDP may have multiple listeners)
    GenUniqueId getApplicationId() const { return mApplicationId; }

    //! Returns true if the connection is reliable
    bool isReliable() const { return mTCP_IO_Ptr != nullptr; }

    //! Specifies that the connection is initialized
    void setInitialized() { mIsInitialized = true; }

    //! Returns 'true' if the connection is initialized.
    bool isInitialized() const { return mIsInitialized; }

    //! Returns 'true' if the connection is in the process of disconnecting
    bool isDisconnecting() const { return mDisconnecting; }

    void setDisconnecting() { mDisconnecting = true; }

    //! Returns 'true' if the clock offset is configured for packet synchronization with this connection
    bool hasClockTranslation() { return mHasClockTranslation; }
    void setHasClockTranslation(bool aUseTranslation) { mHasClockTranslation = aUseTranslation; }

    double getLastTimeStamp() const { return mLastTimeStamp; }
    void setLastTimeStamp(double aTimeStamp) { mLastTimeStamp = aTimeStamp; }

protected:
    // Connections are created and deleted by NXXIO_Interface

    NXXIO_Connection(NXXIO_Interface* aInterfacePtr, PakSocketIO* aIOPtr);
    ~NXXIO_Connection() override;

private:
    std::string mApplicationName;
    GenUniqueId mApplicationId;
    int mConnectionId;
    NXXIO_Interface* mInterfacePtr;
    NXXIO_Connection* mLinkedConnectionPtr;
    PakSocketIO* mIOPtr;
    PakTCP_IO* mTCP_IO_Ptr;
    PakUDP_IO* mUDP_IO_Ptr;
    bool mIsServer;
    bool mIsInitialized;
    bool mDisconnecting;
    bool mHasClockTranslation;
    double mLastTimeStamp;
};

#endif
