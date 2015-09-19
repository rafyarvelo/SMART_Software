#ifndef BCI_C_EEG_IO
#define BCI_C_EEG_IO

#include <QThread>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_singleton.h"
#include "bci_c_connected_device.h"
#include "bci_c_eeg_data.h"

using namespace std;

//This is an abstract EEG interface to be used in the BCI_Package
//Implement these methods in subclasses to connect to the corresponding EEG
class C_EEG_IO : public QThread, public C_ConnectedDevice
{
    Q_OBJECT
public:
    C_EEG_IO()
    {
        this->debugLog = SMART_DEBUG_LOG::Instance();
        QObject::connect(this, SIGNAL(started()), this, SLOT(SetConnected()));
        QObject::connect(this, SIGNAL(finished()), this, SLOT(SetDisconnected()));
    }
	virtual ~C_EEG_IO(){}
	
    //Must implement these members in subclasses
    virtual C_EEG_Data&  getData()  { return eegData; }
    virtual EEG_Frame_t* getFrame() { return eegData.GetFramePtr(); }
    virtual eegTypeEnum  getType() = 0;
    virtual ConnectionStatusType connect() = 0;

signals:
    void EEGDataReady(C_EEG_Data& eegData);

private slots:
    void SetConnected(){ connectionStatus = CONNECTED;}
    void SetDisconnected(){ connectionStatus = NOT_CONNECTED;}

protected:
    SMART_DEBUG_LOG* debugLog;
    C_EEG_Data       eegData;
};

#endif // BCI_C_EEG_IO

