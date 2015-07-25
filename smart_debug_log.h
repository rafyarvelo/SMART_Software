#ifndef SMART_DEBUG_LOG_H
#define SMART_DEBUG_LOG_H

//So we don't have to declare these elsewhere
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//All executables will be one directory below the base
#define BCI_LOG_FILENAME "../debug_logs/bci_debug_log.txt"
#define BRS_LOG_FILENAME "../debug_logs/brs_debug_log.txt"
#define PCC_LOG_FILENAME "../debug_logs/pcc_debug_log.txt"
#define FLASHER_LOG_FILENAME "../debug_logs/flasher_debug_log.txt"

//What happens if we get an error in the Error log class???
//INCEPTION, Thats what happens...

class SMART_DEBUG_LOG
{
private:
	 SMART_DEBUG_LOG()
	 {
	 	bciLog.open(BCI_LOG_FILENAME, ofstream::out);
	 	brsLog.open(BRS_LOG_FILENAME, ofstream::out);
		pccLog.open(PCC_LOG_FILENAME, ofstream::out);
	 	flasherLog.open(FLASHER_LOG_FILENAME, ofstream::out);
	 }//private constructor, use singleton method	
	 
public: 
    ~SMART_DEBUG_LOG()
	{
		bciLog.close();
		brsLog.close();
		pccLog.close();
		flasherLog.close();
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
	
private:
	//fileStreams
	ofstream bciLog;
	ofstream brsLog;
	ofstream pccLog;
	ofstream flasherLog;
};

#endif

