#ifndef BCI_C_EEG_IO
#define BCI_C_EEG_IO

#include <QThread>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_connected_device.h"

typedef struct EEG_Data
{
	byteType* rawData;
	sizeType size;

	static EEG_Data* create(byteType* pData, sizeType size)
	{
		if (!pData || size < 0)
		{
			return new EEG_Data();
		}
		else
		{
			return new EEG_Data(pData, size);
		}
	}
	
	EEG_Data(byteType* data, sizeType size)
	{
		this->rawData = data;
		this->size = size;
	}

	EEG_Data()
	{
		EEG_Data(0,0);
	}

	~EEG_Data()
	{
		delete[] rawData;
	}

} EEG_Data;

//EEG Interface
class C_EEG_IO : public QThread, public C_ConnectedDevice
{
    Q_OBJECT
public:
    C_EEG_IO()
    {
         QObject::connect(this, SIGNAL(started()), this, SLOT(SetConnected()));
         QObject::connect(this, SIGNAL(finished()), this, SLOT(SetDisconnected()));
    }
	virtual ~C_EEG_IO(){}
	
    //Must implement these members in subclasses
	virtual EEG_Data*    getData() = 0;
	virtual eegTypeEnum  getType() = 0;
    virtual ConnectionStatusType connect() = 0;

signals:
    void EEGDataReady(EEG_Data* pData);

private slots:
    void SetConnected(){ connectionStatus = CONNECTED;}
    void SetDisconnected(){ connectionStatus = NOT_CONNECTED;}
};

#endif // BCI_C_EEG_IO

