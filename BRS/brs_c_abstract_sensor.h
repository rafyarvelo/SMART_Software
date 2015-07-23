#ifndef BRS_C_SENSOR_H
#define BRS_C_SENSOR_H

typedef float         SensorDataType;
typedef unsigned char byteType;

class sensorData_t
{
public:
    sensorData_t(char* byteData, int size)
    {
        this->rawData = (byteType*) byteData;
        this->data    = (float*)    byteData;
        this->size    = size;
    }
    sensorData_t()
    {
        this->rawData = 0;
        this->data    = 0;
        this->size    = 0;
    }
    int size;
    byteType*       rawData;
    SensorDataType* data;
};

class C_AbstractSensor
{
public:
     C_AbstractSensor();
    ~C_AbstractSensor();

     virtual void           turnOn ()=0;
     virtual void           turnOff()=0;
     virtual sensorData_t*  GetData()=0;
};

#endif // BRS_C_SENSOR_H

