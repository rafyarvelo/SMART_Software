#ifndef SMART_DEBUG_LOG_H
#define SMART_DEBUG_LOG_H

//So we don't have to declare these elsewhere
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "smart_config.h"
#include "BCI/bci_c_singleton.h"

using namespace std;

//All files will be one directory below the base
#define BCI_LOG_FILENAME         "../debug_files/bci_debug_log.txt"
#define BRS_LOG_FILENAME         "../debug_files/brs_debug_log.txt"
#define PCC_LOG_FILENAME         "../debug_files/pcc_debug_log.txt"
#define FLASHER_LOG_FILENAME     "../debug_files/flasher_debug_log.txt"
#define SERIAL_COMM_LOG_FILENAME "../debug_files/serial_comm_debug_log.txt"

//What happens if we get an error in the Error log class???
//INCEPTION, Thats what happens...

typedef enum
{
    BCI_LOG,
    BRS_LOG,
    PCC_LOG,
    FLASHER_LOG,
    SERIAL_COMM_LOG
} DebugLogType;

class SMART_DEBUG_LOG : public C_Singleton<SMART_DEBUG_LOG>
{
public:
    SMART_DEBUG_LOG()
     {
        bciLog.open(BCI_LOG_FILENAME, ofstream::out);
        brsLog.open(BRS_LOG_FILENAME, ofstream::out);
        pccLog.open(PCC_LOG_FILENAME, ofstream::out);
        flasherLog.open(FLASHER_LOG_FILENAME, ofstream::out);
        serialCommLog.open(SERIAL_COMM_LOG_FILENAME, ofstream::out);
     }

    ~SMART_DEBUG_LOG()
    {
        bciLog.close();
        brsLog.close();
        pccLog.close();
        flasherLog.close();
        serialCommLog.close();
    }

    //return References to file streams when requested
    ofstream& BCI_Log()    { return bciLog; }
    ofstream& BRS_Log()    { return brsLog; }
    ofstream& PCC_Log()    { return pccLog; }
    ofstream& Flasher_Log(){ return flasherLog; }
    ofstream& SerialComm_Log(){ return serialCommLog; }
	
    void println(DebugLogType log,string str, bool printToStdOut=false, bool printToStdErr=false)
    {
        //Print to console if enabled
        if (printToStdOut)
        {
            cout << str << endl;
        }

        if (printToStdErr)
        {
            cerr << str << endl;
        }

        //Track in desired log
        switch (log)
        {
            case BCI_LOG:
                bciLog << str << endl;;
            break;
            case BRS_LOG:
                brsLog << str << endl;;
            break;
            case PCC_LOG:
                pccLog << str << endl;;
            break;
            case FLASHER_LOG:
                flasherLog << str << endl;;
            break;
            case SERIAL_COMM_LOG:
                serialCommLog << str << endl;;
            break;
        }
    }

    void println(DebugLogType log, const char* str, bool printToStdOut=false, bool printToStdErr=false)
    {
        println(log, string(str), printToStdOut, printToStdErr);
    }

private:
    //fileStreams
    ofstream bciLog;
    ofstream brsLog;
    ofstream pccLog;
    ofstream flasherLog;
    ofstream serialCommLog;
};

#endif

