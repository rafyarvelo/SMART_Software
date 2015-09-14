#ifndef EEG_IO_EMOTIV
#define EEG_IO_EMOTIV

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_eeg_io.h"
#include "emokit.h"

//Constants
#define EMOTIV_FRAME_SIZE sizeof(emokit_frame)
#define READ_TIMEOUT      2000 //ms
#define MAX_TIMEOUTS      10 //Give up after 20 seconds of timeouts
#define MIN_FRAMES_NEEDED 10 //Need at least 10 frames for a good sample

enum EmotivReadStatus
{
    EMOKIT_READ_TIMEOUT=0,
    EMOKIT_READ_SUCCESS
};

class C_EEG_IO_EMOTIV : public C_EEG_IO
{
    Q_OBJECT
public:
             C_EEG_IO_EMOTIV();
    virtual ~C_EEG_IO_EMOTIV();
    static C_EEG_IO_EMOTIV* Instance() {return new C_EEG_IO_EMOTIV;}
	
    virtual C_EEG_Data&  getData() { return eegData; }
	virtual eegTypeEnum  getType(){ return EEG_TYPE_EMOTIV; }
    virtual ConnectionStatusType connect();

    virtual void run();       //Thread Execution

public slots:
    void clearEEGData();

private:
    C_EEG_Data eegData; //All Emotiv Data goes here

    struct emokit_device* m_device;

    //Number of times we have missed a read
    int m_timeout_count;

    //Number of EPOC Devices Connected
    int m_device_count;

    //Status for Open/Close operations
    int m_resource;
};

#endif
