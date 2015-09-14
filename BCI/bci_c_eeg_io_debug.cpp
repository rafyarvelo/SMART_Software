#include "bci_c_eeg_io_debug.h"

//C_EEG_IO_DEBUG::C_EEG_IO_DEBUG()
//	: mDebugData(0)
//{
//	srand(static_cast<unsigned int>(time(0))); //seed value to system clock
//    connectionStatus = CONNECTED;//Just debugging here...
//}

//C_EEG_IO_DEBUG::~C_EEG_IO_DEBUG()
//{
//	if (mDebugData)
//	{
//		delete mDebugData;
//	}
//}

//C_EEG_IO_DEBUG* C_EEG_IO_DEBUG::Instance()
//{
//	C_EEG_IO_DEBUG* ptr = new C_EEG_IO_DEBUG();//Create new Instance
	
//	if (!ptr)
//	{

//		cout           << "ERROR! C_EEG_IO Could not be instantiated!" << endl;
//        debugLog->BCI_Log() << "ERROR! C_EEG_IO Could not be instantiated!" << endl;
//		return 0;
//	}
//	else
//	{
//		return ptr;
//	}
//}

////void C_EEG_IO_DEBUG::setDebugData(byteType* pData, sizeType size)
////{
////	C_EEG_Data* data = C_EEG_Data::create(pData, size);
////	setDebugData(data);
////}

////void C_EEG_IO_DEBUG::setDebugData(C_EEG_Data* pData)
////{
////	mDebugData = pData;
////}

//////Random Debug Data for our EEG
////C_EEG_Data* C_EEG_IO_DEBUG::createRandomData()
////{
////	unsigned int r  = rand();
////	sizeType size   = (r + MIN_DEBUG_DATA_SIZE) % MAX_DEBUG_DATA_SIZE;
////	byteType* pData = new byteType[size];
	
////	//create Random Data
////    for (sizeType i = 0; i < size; i++)
////	{
////		pData[i] = (char) (rand() % 256);//255 is max size for 8 bits
////	}
	
////	return C_EEG_Data::create(pData, size);
////}

////C_EEG_Data* C_EEG_IO_DEBUG::getData()
////{
////	mDebugData = createRandomData();
////	return mDebugData;
////}

