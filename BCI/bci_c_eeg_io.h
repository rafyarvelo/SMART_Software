#ifndef BCI_C_EEG_IO
#define BCI_C_EEG_IO

#include "../smart_config.h"

typedef unsigned char  byteType;
typedef unsigned short wordType;
typedef unsigned int   dWordType;

typedef struct EEG_Data
{
	byteType* rawData;
	unsigned int size;

	EEG_Data(byteType* data, unsigned int size)
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
	         C_EEG_IO();
	virtual ~C_EEG_IO();
	
	virtual EEG_Data*    getData() = 0;
	virtual eegTypeEnum  getType() = 0;

private:
	EEG_Data*    pData;
	bool         connectionStatus;
};

#endif // BCI_C_EEG_IO

