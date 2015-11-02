#ifndef C_BRSH_IO_H
#define C_BRSH_IO_H

#include <QThread>
#include <QTimer>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_singleton.h"
#include "bci_c_connected_device.h"
#include "bci_c_safequeue.h"
#include "bci_c_framegenerator.h"

//Store the Received BRS Frames in a Semaphore Protected Circular Buffer
typedef C_SafeQueue<BRS_Frame_t> brsFrameBufferType;

//The TM Frames are also stored in a Circular Buffer
typedef C_SafeQueue<TM_Frame_t> tmFrameBufferType;

//Abstract BRS IO Class, essentially just a worker-object
class C_BRSH_IO : public QObject, public C_ConnectedDevice
{
    Q_OBJECT
public:
     C_BRSH_IO();
     virtual ~C_BRSH_IO();

    //Connect to BRSHs
    virtual ConnectionStatusType connect() = 0;

    //The Rate we will execute the BRSH IO Task (10 Hz)
    static const uint16_t EXECUTION_RATE;

public slots:
    //Start Getting Data
    void begin();

    //Try to Retrieve a Frame, return true and emit BRSFrameReceived() if successful
    virtual bool fetchBRSFrame() = 0;

    //Send a Frame to the BRSH
    virtual void SendTMFrame(tmFrameBufferType*) = 0;

signals:
    //Give the Receiver access to the buffer where we will store the Frames
    void BRSFrameReceived(brsFrameBufferType* pBRSFrameBuffer);

protected:
    QTimer       mTimer;
    QThread      mThread;
    SMART_DEBUG_LOG*   debugLog;

    //Store all of the BRS Frames here
    brsFrameBufferType brsFrameBuffer;
    BRS_Frame_t        mCurrentBRSFrame; //Only the Latest Frame
};

#endif // C_BRSH_IO_H
