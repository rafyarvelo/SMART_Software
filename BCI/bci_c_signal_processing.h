#ifndef BCI_C_SIGNAL_PROCESSING_H
#define BCI_C_SIGNAL_PROCESSING_H

#include <atomic>
#include <thread>

#include <QThread>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_eeg_io.h"

#define MIN_CONTACT_QUALITY 2000
#define MAX_CONTACT_QUALITY 6000

#define MIN_FRAMES_NEEDED 10

//Final Score as an Unsigned integer
typedef unsigned short ProcessingScore_t;

//Since the Signal Processing will likely occur at a faster rate then we can
//send commands, we'll just stick the Processing Results in a Circular Buffer
//and then Send out a Signal with Reference to the Buffer whenever we add a result.
//Since we will used a Qt::QueuedConnection for our Signal along with FIFO access to
//our buffer, we will be all good to go here
typedef C_SafeQueue<ProcessingResult_t> resultsBufferType;

class C_SignalProcessing : public QObject , public C_Singleton<C_SignalProcessing>
{
    Q_OBJECT
public:

    C_SignalProcessing();
   ~C_SignalProcessing();	

   void recvLoop();

signals:
    void eegDataProcessed(resultsBufferType* pProcessingResults);

public slots:
    void processFrame(EEG_Frame_t& frame);
	
private:
    SMART_DEBUG_LOG*   debugLog;
    QThread            mThread;

    resultsBufferType  mResultsBuf;

    std::thread        mRecvThread;
    std::atomic_bool   mIsAlive;
};

#endif // BCI_C_SIGNAL_PROCESSING_H

