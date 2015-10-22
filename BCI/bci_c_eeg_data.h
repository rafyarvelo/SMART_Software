#ifndef BCI_C_EEG_DATA_H
#define BCI_C_EEG_DATA_H

#include <QObject>
#include <QList>
#include "smart_config.h"
#include "smart_data_types.h"
#include "bci_c_safequeue.h"

//This is the maximum amount of frames before we'll store
//before we wrap around in our circular buffer
#define EEG_BUFFER_SIZE 30

//This is just a simple wrapper around our SafeQueue Class to
//safely store EEG Frames in a FIFO
class C_EEG_Data
{
public:
     C_EEG_Data(unsigned int bufferSize = EEG_BUFFER_SIZE);
    ~C_EEG_Data();

    //Add a new frame of EEG Data
    void AddFrame(EEG_Frame_t* eegFrame);

    //Return Last Frame by Default
    EEG_Frame_t* GetFramePtr(int index = -1);

    //Clear all of the Frames
    void clear();

    //Return the number of Frames Available
    sizeType size() { return (sizeType) frames->itemsAvailable(); }

    //Assignment Operator
    C_EEG_Data& operator =(C_EEG_Data& rhs);

private:
    C_SafeQueue<EEG_Frame_t*>* frames;
};

#endif // BCI_C_EEG_DATA_H
