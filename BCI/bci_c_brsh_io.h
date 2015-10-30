#ifndef C_BRSH_IO_H
#define C_BRSH_IO_H

#include <QThread>
#include <QTimer>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_singleton.h"
#include "bci_c_connected_device.h"
#include "bci_c_tm.h"
#include "bci_c_framegenerator.h"

#define MAX_MISS_COUNT 5  //Disconnect after 5 misses

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
    virtual void SendTMFrame(TM_Frame_t* pFrame) = 0;

signals:
    void BRSFrameReceived(BRS_Frame_t* frame);

protected:
    QTimer       mTimer;
    QThread      mThread;
    SMART_DEBUG_LOG* debugLog;
};

#endif // C_BRSH_IO_H
