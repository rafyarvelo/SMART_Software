#ifndef BCI_C_EEG_IO
#define BCI_C_EEG_IO

#include <QThread>
#include <QTimer>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_singleton.h"
#include "bci_c_connected_device.h"
#include "bci_c_safequeue.h"
#include "bci_c_binary_parser.h"

//Store the EEG Frames in a Circular Buffer
typedef C_SafeQueue<EEG_Frame_t> eegFrameBufferType;

//This is an abstract EEG interface to be used in the BCI_Package
//Implement these methods in subclasses to connect to the corresponding EEG
class C_EEG_IO : public QObject, public C_ConnectedDevice
{
    Q_OBJECT
public:
    C_EEG_IO();
    virtual ~C_EEG_IO();

    //Must implement these members in subclasses
    virtual eegTypeEnum  getType() = 0;
    virtual ConnectionStatusType connect() = 0;

    //The Rate we will execute the EEG IO Task (50 Hz)
    static const uint16_t EXECUTION_RATE;

    void RecordTMToFile(const QString& filename);

public slots:
    //Start Getting Data
    void begin();

    //Try to Get an EEG Frame, return true and emit EEG Frame Received if Successful
    virtual bool fetchEEGFrame() = 0;

signals:
    void EEGFrameReceived(eegFrameBufferType* pEEGFrameBuffer);

protected:
    SMART_DEBUG_LOG* debugLog;

    //Used to Record TM to File
    C_BinaryParser* eegTMFile;

    //Log TM Flag
    bool recordTM;

    //Store All EEG Frames here
    eegFrameBufferType eegFrameBuffer;
    EEG_Frame_t mCurrentEEGFrame; //Only the Latest Frame

private:
    QTimer           mTimer; //How we will implement our execution
    QThread          mThread;

};

#endif // BCI_C_EEG_IO
