#ifndef BCI_C_TM_HEADER_NAMES_H
#define BCI_C_TM_HEADER_NAMES_H

#include <QStringList>
#include <QString>
#include "smart_config.h"

struct C_HEADER_NAMES
{
    QStringList tmFrameNames;
    QStringList eegFrameNames;
    QStringList brsFrameNames;
    QStringList gpsData;
    QStringList usData;

    C_HEADER_NAMES()
    {
        gpsData << "latitude"
                << "longitude"
                << "altitude"
                << "groundSpeed";

        usData  << "rangeFront"
                << "rangeBack";

        brsFrameNames << gpsData
                      << usData
                      << "remoteCommand";

        eegFrameNames << "eegType"
                      << "counter"

                      //Electrode Data
                      << "F3"  << "FC6" << "P7"  << "T8"
                      << "F7"  << "F8"  << "T7"  << "P8"
                      << "AF4" << "F4"  << "AF3" << "02"
                      << "01"  << "FC5"

                      //32 Electrodes for Nautilus EEG
                      #if DEFAULT_EEG_TYPE != EEG_TYPE_EMOTIV
                      << "spare1"  << "spare2"  << "spare3"
                      << "spare4"  << "spare5"  << "spare6"
                      << "spare7"  << "spare8"  << "spare9"
                      << "spare10" << "spare11" << "spare12"
                      << "spare13" << "spare14" << "spare15"
                      << "spare16" << "spare17" << "spare18"
                      #endif

                      //Contact Quality
                      << "F3_CQ"  << "FC6_CQ" << "P7_CQ"  << "T8_CQ"
                      << "F7_CQ"  << "F8_CQ"  << "T7_CQ"  << "P8_CQ"
                      << "AF4_CQ" << "F4_CQ"  << "AF3_CQ" << "02_CQ"
                      << "01_CQ"  << "FC5_CQ"

                      //32 Electrodes for Nautilus EEG
                      #if DEFAULT_EEG_TYPE != EEG_TYPE_EMOTIV
                      << "spare1"  << "spare2"  << "spare3"
                      << "spare4"  << "spare5"  << "spare6"
                      << "spare7"  << "spare8"  << "spare9"
                      << "spare10" << "spare11" << "spare12"
                      << "spare13" << "spare14" << "spare15"
                      << "spare16" << "spare17" << "spare18"
                      #endif

                      //The rest of the crap
                      << "gyroX"
                      << "gyroY"
                      << "batteryPercentage";

        tmFrameNames  << "timeStamp"
                      << "BCI_State"
                      << eegFrameNames
                      << brsFrameNames
                      << "LED_Freq_Forward"
                      << "LED_Freq_Backward"
                      << "LED_Freq_Right"
                      << "LED_Freq_Left"
                      << "eegConnectionStatus"
                      << "brsConnectionStatus"
                      << "pccConnectionStatus"
                      << "flasherConnectionStatus";
    }
    ~C_HEADER_NAMES(){}
};

#endif // BCI_C_TM_HEADER_NAMES_H
