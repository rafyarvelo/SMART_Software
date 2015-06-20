#ifndef BCI_C_TM
#define BCI_C_TM

#include <QByteArray>
#include "../../BRS/brs_c_abstract_sensor.h"

class C_TM
{
    sensorData_t GPS_Data;
    sensorData_t US_Data;

    sensorData_t
    C_TM(sensorData_t* gpsData=0, sensorData_t* usData=0)
    {
    }

};

#endif // BCI_C_TM

