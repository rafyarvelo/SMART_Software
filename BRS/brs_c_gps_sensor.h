#ifndef BRS_C_GPS_SENSOR
#define BRS_C_GPS_SENSOR

#include "brs_c_abstract_sensor.h"

class C_GPS_Sensor : public C_AbstractSensor
{
public:
    virtual void           turnOn();
    virtual void           turnOff();
    virtual sensorData_t*  GetData();

};

#endif // BRS_C_GPS_SENSOR

