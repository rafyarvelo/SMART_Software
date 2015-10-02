#ifndef BRS_SENSOR_DATA_H
#define BRS_SENSOR_DATA_H

#define MAX_RANGE_TO_OBJECT 6 //Meters

struct GPS_Data_t
{
    float latitude;
    float longitude;
    float altitude;
    float groundSpeed;
};

struct US_Data_t
{
    float rangeToObject;
};

#endif //BRS_SENSOR_DATA_H
