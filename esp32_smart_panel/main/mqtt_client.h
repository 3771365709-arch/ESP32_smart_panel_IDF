#ifndef _MQTT_CLIENT_H_
#define _MQTT_CLIENT_H_

#include <stdbool.h>

void mqtt_start(void);
bool mqtt_is_connected(void);
void mqtt_publish_sensor(float temp, float humi);

#endif
