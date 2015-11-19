#ifndef BCI_C_MCU_IO
#define BCI_C_MCU_IO

#include <QThread>
#include <QTimer>
#include "bci_c_rvs.h"
#include "bci_c_connected_device.h"
#include "bci_c_singleton.h"
#include "../smart_config.h"
#include "../smart_debug_log.h"

class C_Flasher_IO : public QObject, public C_ConnectedDevice
{
    Q_OBJECT
public:
     C_Flasher_IO();
    ~C_Flasher_IO();

public slots:
    //Start IO 
    void begin();
    virtual ConnectionStatusType connect() = 0;
    virtual void SendRVS() = 0;

protected:
     SMART_DEBUG_LOG* debugLog;

private:
     static const uint16_t EXECUTION_RATE;
     QTimer  mTimer;
     QThread mThread;
};

#endif // BCI_C_MCU_IO

