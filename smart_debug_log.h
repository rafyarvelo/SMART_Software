#ifndef SMART_DEBUG_LOG_H
#define SMART_DEBUG_LOG_H

//So we don't have to declare these elsewhere
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//All files will be one directory below the base
#define BCI_LOG_FILENAME         "../debug_files/bci_debug_log.txt"
#define BRS_LOG_FILENAME         "../debug_files/brs_debug_log.txt"
#define PCC_LOG_FILENAME         "../debug_files/pcc_debug_log.txt"
#define FLASHER_LOG_FILENAME     "../debug_files/flasher_debug_log.txt"
#define SERIAL_COMM_LOG_FILENAME "../debug_files/serial_comm_debug_log.txt"

//What happens if we get an error in the Error log class???
//INCEPTION, Thats what happens...

class SMART_DEBUG_LOG
{
private:
	 SMART_DEBUG_LOG()//private constructor, use singleton method
	 {
	 	bciLog.open(BCI_LOG_FILENAME, ofstream::out);
	 	brsLog.open(BRS_LOG_FILENAME, ofstream::out);
		pccLog.open(PCC_LOG_FILENAME, ofstream::out);
	 	flasherLog.open(FLASHER_LOG_FILENAME, ofstream::out);
        serialCommLog.open(SERIAL_COMM_LOG_FILENAME, ofstream::out);
	 }	
	 
public: 
    ~SMART_DEBUG_LOG()
	{
		bciLog.close();
		brsLog.close();
		pccLog.close();
		flasherLog.close();
        serialCommLog.close();
	}
    
	static SMART_DEBUG_LOG* Instance()
	{
		static SMART_DEBUG_LOG* ptr = 0;
		
		if (!ptr)//only create one instance of the log
		{
			ptr = new SMART_DEBUG_LOG();
		}	
		
		return ptr;
	}
	
	//return References to file streams when requested
	ofstream& BCI_Log()    { return bciLog; }
	ofstream& BRS_Log()    { return brsLog; }
	ofstream& PCC_Log()    { return pccLog; }
	ofstream& Flasher_Log(){ return flasherLog; }	
    ofstream& SerialComm_Log(){ return serialCommLog; }
	
private:
	//fileStreams
	ofstream bciLog;
	ofstream brsLog;
	ofstream pccLog;
	ofstream flasherLog;
    ofstream serialCommLog;
};

#endif

