#ifndef BCI_C_EEG_DATA_H
#define BCI_C_EEG_DATA_H

#include <QObject>
#include <QList>
#include "smart_config.h"
#include "emokit.h"

//This is here to show the Structure of the an Emotiv Frame, which is our default EEG
/*
    struct emokit_contact_quality { //values > 4000 are good
        short F3, FC6, P7, T8, F7, F8, T7, P8, AF4, F4, AF3, O2, O1, FC5;
    };

    struct emokit_frame {
        unsigned char counter; //loops from 0 to 128 (129 values)
        int F3, FC6, P7, T8, F7, F8, T7, P8, AF4, F4, AF3, O2, O1, FC5; //raw data values
        struct emokit_contact_quality cq;
        char gyroX, gyroY;
        unsigned char battery; //percentage of full charge, read on counter=128
    };
*/

//This is the maximum amount of readings we will support
#define MAX_EEG_ELECTRODES 32

//This is the maximum amount of frames before we're full
#define MAX_EEG_FRAMES 500

//For Easy access to Emotiv Electrode Data
typedef enum Emotiv_Electrodes
{
    F3=0, FC6, P7, T8 , F7,
    F8  , T7 , P8, AF4, F4,
    AF3 , O2 , O1, FC5, NUM_EMOTIV_ELECTRODES
} Emotiv_Electrodes;

//This struct defines what a single frame of EEG Data looks like
typedef struct EEG_Frame_t
{
    //The type of EEG that the frame belongs to, appended by EEG IO class
    eegTypeEnum eegType;

    quint32 counter; //Value counter

    //Data from Each electrode, defaults to 0
    qint32 electrodeData[MAX_EEG_ELECTRODES];

    //Contact Quality from Each electrode, defaults to 0
    qint16 contactQuality[MAX_EEG_ELECTRODES];

    //Gyro Values
    qint8 gyroX;
    qint8 gyroY;

    //Percentage of Full Battery Charge
    quint8 batteryPercentage;

    //Default Frame
    EEG_Frame_t()
    {
        eegType = DEFAULT_EEG_TYPE;

        for (int i = 0; i < MAX_EEG_ELECTRODES;i++)
        {
            electrodeData[i]  = 0;
            contactQuality[i] = 0;
        }

        gyroX = 0;
        gyroY = 0;
        batteryPercentage = 0;
    }

    //Factory Method
    static EEG_Frame_t* create()
    {
        return new EEG_Frame_t;
    }

    //Easy conversion from Emotiv since thats the default EEG
    static EEG_Frame_t* fromEmotivFrame(struct emokit_frame* emotiv_frame)
    {
        EEG_Frame_t* frame = EEG_Frame_t::create();

        //Initialize frame from Emotiv Data
        frame->eegType = EEG_TYPE_EMOTIV;
        frame->counter = (quint32) emotiv_frame->counter;

        //Electrode Data from Emotiv Frame
        memcpy((void*) &frame->electrodeData[0], (void*) &emotiv_frame->F3,
                (size_t) sizeof(int) * NUM_EMOTIV_ELECTRODES);

        //Contact Quality from Emotiv Frame
        memcpy((void*) &frame->contactQuality[0], (void*) &emotiv_frame->cq,
                (size_t) sizeof(struct emokit_contact_quality));

        //The rest of the Emotiv Data
        frame->gyroX = emotiv_frame->gyroX;
        frame->gyroY = emotiv_frame->gyroY;
        frame->batteryPercentage = emotiv_frame->battery;

        return frame;
    }

} EEG_Frame_t;

class C_EEG_Data : public QObject
{
    Q_OBJECT
public:
     explicit C_EEG_Data(QObject* parent=0);
    ~C_EEG_Data();

    //Factory Constructor
    static C_EEG_Data* Instance() { return new C_EEG_Data; }

    //Add a new frame of EEG Data
    void AddFrame(EEG_Frame_t* eegFrame);

    //Return Last Frame by Default
    EEG_Frame_t& GetFrame   (int index = -1);
    EEG_Frame_t* GetFramePtr(int index = -1);

    //Clear all of the Frames
    void clear();

    //check if we have reached the max amount of data
    bool isFull() { return eegDataFull; }

    //Return the number of Frames Available
    sizeType size() { return currentFrame; }

    //Assignment Operator
    C_EEG_Data& operator =(C_EEG_Data& rhs);

    //Signal when we can't take anymore data
signals:
    void EEG_Data_Full();

private:
    sizeType            currentFrame;
    bool                eegDataFull;
    QList<EEG_Frame_t*> frames;
};

#endif // BCI_C_EEG_DATA_H
