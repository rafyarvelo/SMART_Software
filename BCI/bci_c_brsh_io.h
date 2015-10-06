#ifndef C_BRSH_IO_H
#define C_BRSH_IO_H

#include <QThread>
#include <QTimer>
#include <QSemaphore>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_singleton.h"
#include "bci_c_connected_device.h"
#include "bci_c_tm.h"
#include "bci_c_framegenerator.h"

#define BRS_FRAME_MUTEX 1 //Initialize our mutex to have 1 fram available

#define MAX_MISS_COUNT 5  //Disconnect after 5 misses

//Abstract BRS IO Class, essentially just a worker-object
class C_BRSH_IO : public QObject, public C_ConnectedDevice
{
    Q_OBJECT
public:
     C_BRSH_IO();
     virtual ~C_BRSH_IO();

    //Return the latest BRS Data
    BRS_Frame_t* GetLatestBRSFramePtr();

    //Connect to BRSHs
    virtual ConnectionStatusType connect() = 0;

    //The Rate we will execute the BRSH IO Task (10 Hz)
    static const u_int16_t EXECUTION_RATE;

    //Protect BRS Frame From Concurrent Access
    static QSemaphore*  pBRSFrameMutex;

public slots:
    //Start Getting Data
    void begin();

    //Try to Retrieve a Frame, return true and emit BRSFrameReceived() if successful
    virtual bool fetchBRSFrame() = 0;

    //Send a Frame to the BRSH
    virtual void SendTMFrame(TM_Frame_t* pFrame) = 0;

signals:
    void BRSFrameReceived(BRS_Frame_t* brsFrame);

protected:
    BRS_Frame_t* pLatestBRSFrame;
    QTimer       mTimer;
    QThread      mThread;
};

#endif // C_BRSH_IO_H
