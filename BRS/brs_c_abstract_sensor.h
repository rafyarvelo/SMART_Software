#ifndef BRS_C_SENSOR_H
#define BRS_C_SENSOR_H

typedef float         SensorDataType;
typedef unsigned char byteType;

typedef struct sensorData_t
{
    int size;
    byteType*       rawData;
    SensorDataType* data;

    sensorData_t(char* byteData, int size)
    {
        this->rawData = (byteType*) byteData;
        this->data    = (float*)    byteData;
        this->size    = size;
    }
}sensorData_t;

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

