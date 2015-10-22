#ifndef BCI_C_FILEPARSER_H
#define BCI_C_FILEPARSER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include "bci_c_tm.h"
#include "bci_c_eeg_data.h"

#define READ_ONLY  true
#define WRITE_ONLY false

//General Format of Input/Output Files
namespace FILE_FORMAT
{
    //EEG Specific XML Tags
    const QString EEG_DATA    = "eegData";
    const QString EEG_FRAME   = "eegFrame";
    const QString EEG_TYPE    = "eegType";
    const QString EEG_COUNTER = "eegCounter";
    const QString EEG_READING = "eegElectrodeData";
    const QString EEG_CONTACT = "eegContactQuality";
    const QString EEG_GYROX   = "eegGyroX";
    const QString EEG_GYROY   = "eegGyroY";
    const QString EEG_BATTERY = "eegBatteryPercent";

    //BRS Specific XML Tags
    const QString TM_DATA     = "tmData";
    const QString TM_FRAME    = "tmFrame";
    const QString SENSOR_DATA = "sensorData";
    const QString PCC_COMMAND = "pccCommand";
    const QString LED_GROUD   = "ledGroup";
    const QString CONN_STATUS = "connectionStatus";

    //Generic XML TAGS
    const QString NAME   = "name";
    const QString TIME   = "time";
    const QString VALUE  = "value";
    const QString STATUS = "status";

    //Map Int to String
    const QString BCI_STATES[]    = {"BCI_OFF", "BCI_STANDBY", "BCI_PROCESSING", "BCI_READY"};
    const QString LED_GROUPS[]    = {"Forward", "Backward", "Right", "Left"};
    const QString EEG_TYPES[]     = {"EEG_Emotiv", "EEG_Nautilus", "EEG_Debug"};
    const QString CONN_STATUSES[] = {"NOT_CONNECTED", "CONNECTED"};
    const QString CONFIDENCES[]   = {"Unsure", "Moderate", "Likely", "Absolute"};
};

//Abstract Telemetry File Parser Class
class C_AbstractParser
{
public:
    C_AbstractParser(const QString& filename, QIODevice::OpenModeFlag openMode);
    virtual ~C_AbstractParser();

    //Read/Write Individual Frames
    virtual EEG_Frame_t* readEEGFrame() = 0;
    virtual TM_Frame_t*  readTMFrame() = 0;
    virtual void writeEEGFrame(EEG_Frame_t* frame) = 0;
    virtual void writeTMFrame(TM_Frame_t* frame) = 0;

protected:
    //Input/Output File Pointer
    QFile* fp;
    QString m_filename;
};

#endif // BCI_C_FILEPARSER_H
