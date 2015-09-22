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
#define READ_TIMEOUT      1000 //ms
#define MAX_TIMEOUTS      5 //Give up after 5 timeouts

enum EmotivReadStatus
{
    EMOKIT_READ_TIMEOUT=0,
    EMOKIT_READ_SUCCESS
};

class C_EEG_IO_EMOTIV : public C_EEG_IO , public C_Singleton<C_EEG_IO_EMOTIV>
{
    Q_OBJECT
public:
             C_EEG_IO_EMOTIV();
    virtual ~C_EEG_IO_EMOTIV();

    virtual eegTypeEnum  getType(){ return EEG_TYPE_EMOTIV; }
    virtual ConnectionStatusType connect();

    //Easy Conversion from Emokit Frames to EEG Frames
    EEG_Frame_t* fromEmotivFrame(emokit_frame& frame) { return fromEmotivFrame(&frame); }
    EEG_Frame_t* fromEmotivFrame(emokit_frame* frame);
public slots:
    virtual bool fetchEEGFrame();
    void clearEEGData() { eegData.clear(); }

private:
    struct emokit_device* m_device;

    //Number of EPOC Devices Connected
    int m_device_count;

    //Status for Open/Close operations
    int m_resource;
};

#endif
