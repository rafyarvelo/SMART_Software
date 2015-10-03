#ifndef BCI_C_EEG_DATA_H
#define BCI_C_EEG_DATA_H

#include <QObject>
#include <QList>
#include "smart_config.h"
#include "smart_data_types.h"

//This is the maximum amount of frames before we're full
#define MAX_EEG_FRAMES 30

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

    //Return the number of Frames Available
    sizeType size() { return frames.size(); }

    //Assignment Operator
    C_EEG_Data& operator =(C_EEG_Data& rhs);

    //Signal when we can't take anymore data
signals:
    void EEG_Data_Full();

private:
    QList<EEG_Frame_t*> frames;
};

#endif // BCI_C_EEG_DATA_H
