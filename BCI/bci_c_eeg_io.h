#ifndef BCI_C_EEG_IO
#define BCI_C_EEG_IO

#include "../smart_config.h"
#include "../smart_debug_log.h"

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
class C_EEG_IO
{
public:
	         C_EEG_IO(){}
	virtual ~C_EEG_IO(){}
	
	virtual EEG_Data*    getData() = 0;
	virtual eegTypeEnum  getType() = 0;
	ConnectionStatusType GetConnectionStatus() {return connectionStatus;}

protected:
	bool         connectionStatus;
};

#endif // BCI_C_EEG_IO

