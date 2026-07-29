#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <stdint.h>

void sensor_init(void);
float sensor_get_temperature(void);
float sensor_get_humidity(void);
void sensor_task(void *pvParameters);

#endif
