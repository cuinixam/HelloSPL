#ifndef ASIC_DATA_H
#define ASIC_DATA_H


#include "rte.h"

typedef struct
{
    sint32_t Voltage[4];
    uint8_t Register[2];
} ASIC_SensorData_t;


typedef struct
{
	ASIC_SensorData_t sensor_data;
} ASIC_data_t;

extern ASIC_data_t asic_data;

#endif