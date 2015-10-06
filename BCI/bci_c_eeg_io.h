#ifndef BCI_C_EEG_IO
#define BCI_C_EEG_IO

#include <QThread>
#include <QTimer>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_singleton.h"
#include "bci_c_connected_device.h"
#include "bci_c_eeg_data.h"

using namespace std;

//Need at least 10 frames for a good sample
#define MIN_FRAMES_NEEDED 10

//This is an abstract EEG interface to be used in the BCI_Package
//Implement these methods in subclasses to connect to the corresponding EEG
class C_EEG_IO : public QObject, public C_ConnectedDevice
{
    Q_OBJECT
public:
    C_EEG_IO();
    virtual ~C_EEG_IO();

    //Basic Getters
    virtual C_EEG_Data&  GetData()     { return eegData; }
    virtual EEG_Frame_t& GetFrame()    { return eegData.GetFrame(); }
    virtual EEG_Frame_t* GetFramePtr() { return eegData.GetFramePtr(); }

    //Must implement these members in subclasses
    virtual eegTypeEnum  getType() = 0;
    virtual ConnectionStatusType connect() = 0;

    //The Rate we will execute the EEG IO Task (50 Hz)
    static const u_int16_t EXECUTION_RATE;

public slots:
    //Start Getting Data
    void begin();

    //Try to Get an EEG Frame, return true and emit EEG Frame Received if Successful
    virtual bool fetchEEGFrame() = 0;

signals:
    void EEGFrameReceived(EEG_Frame_t* frame);

    //Emit when we have received 10 frames successfully
    void EEGDataReady(C_EEG_Data& eegData);

protected:
    SMART_DEBUG_LOG* debugLog;
    C_EEG_Data       eegData;
    QTimer           mTimer; //How we will implement our execution
    QThread          mThread;
};

#endif // BCI_C_EEG_IO

