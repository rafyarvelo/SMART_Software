#include "bci_c_eeg_io_emotiv.h"

//Constructor
C_EEG_IO_EMOTIV::C_EEG_IO_EMOTIV()
    : m_timeout_count(0),
      m_resource(0)
{
    debugLog = SMART_DEBUG_LOG::Instance();

    //Create New Emotiv Device, Wait to connect until connect() is called
    m_device       = emokit_create();
    m_device_count = emokit_get_count(m_device, EMOKIT_VID, EMOKIT_PID);

    //Delete EEG Data when it gets full
    QObject::connect(&eegData, SIGNAL(EEG_Data_Full()), this, SLOT(clearEEGData()));
}

//~Destructor
C_EEG_IO_EMOTIV::~C_EEG_IO_EMOTIV()
{
    //Close and Delete the Emotiv struct
    emokit_close(m_device);
    emokit_delete(m_device);
}

ConnectionStatusType C_EEG_IO_EMOTIV::connect()
{
    debugLog->BCI_Log() << "Trying to connect to Emotiv EEG... " << endl;

    //Connect to Emotiv Device
    m_resource = emokit_open(m_device, EMOKIT_VID, EMOKIT_PID, 1);

    //If we couldn't connect, try one more time
    if(m_resource != 0)
    {
        debugLog->BCI_Log() << "Retrying Emotiv Connect... "<< endl;

        //Delete old device
        emokit_close(m_device);
        emokit_delete(m_device);

        //Create New Device
        m_device       = emokit_create();
        m_resource     = emokit_open(m_device, EMOKIT_VID, EMOKIT_PID, 0);
        m_device_count = emokit_get_count(m_device, EMOKIT_VID, EMOKIT_PID);

        //We Couldn't Connect:(
        if (m_resource != 0)
        {
            cerr                << "CANNOT CONNECT: " << m_resource << endl;
            debugLog->BCI_Log() << "CANNOT CONNECT: " << m_resource << endl;
            return NOT_CONNECTED;
        }
    }

    //If we reach here, connection was successful
    cout                << "Connected to Emotiv EEG " << endl;
    debugLog->BCI_Log() << "Connected to Emotiv EEG " << endl;
    debugLog->BCI_Log() << "Current EPOC Devices Connected: " << m_device_count << endl;

    return CONNECTED;
}

//Thread Execution
void C_EEG_IO_EMOTIV::run()
{
    struct emokit_frame frame; //buffer to hold each EEG Frame
    EmotivReadStatus readStatus;
    int newFrameCount = 0;

    debugLog->BCI_Log() << "EEG Thread Started: " << QThread::currentThreadId() << endl;

    //Connect if we aren't connected yet
    if (m_resource != 0 || connectionStatus == NOT_CONNECTED)
    {
        this->connect();
    }

    //Execute Thread Mainloop
    while (m_timeout_count < MAX_TIMEOUTS)
    {
        //Attempt to read the next frame
        readStatus = (EmotivReadStatus) emokit_read_data_timeout(m_device, READ_TIMEOUT);

        //Check if we timed out
        if (readStatus == EMOKIT_READ_TIMEOUT)
        {
            debugLog->BCI_Log() << "Emotiv Read Timeout: Count = " << ++m_timeout_count << endl;
        }
        else
        {
            //Get the current EEG Frame and add it to our data
            frame = emokit_get_next_frame(m_device);
            eegData.AddFrame(EEG_Frame_t::fromEmotivFrame(&frame));
            newFrameCount++;
        }

        //Signal that our EEG Data is Ready every 10 Frames
        if (newFrameCount >= MIN_FRAMES_NEEDED)
        {
            emit EEGDataReady(eegData);
            newFrameCount = 0;
        }
    }

    if (m_timeout_count >= MAX_TIMEOUTS)
    {
        cout                << "Max Time Outs reached waiting for Emotiv." << endl;
        debugLog->BCI_Log() << "Max Time Outs reached waiting for Emotiv." << endl;
    }
}

void C_EEG_IO_EMOTIV::clearEEGData()
{
    eegData.clear();
}
