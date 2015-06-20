#ifndef BRS_C_US_SENSOR
#define BRS_C_US_SENSOR

#include "brs_c_abstract_sensor.h"

class C_US_Sensor : public C_AbstractSensor
{
public:
     C_US_Sensor();
    ~C_US_Sensor();

    virtual void           turnOn();
    virtual void           turnOff();
    virtual sensorData_t*  GetData();

};

#endif // BRS_C_US_SENSOR

