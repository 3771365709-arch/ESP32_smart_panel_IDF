#ifndef _RELAY_H_
#define _RELAY_H_

#include <stdbool.h>
#include "driver/gpio.h"

#define RELAY_PIN  GPIO_NUM_26

void relay_init(void);
void relay_set(bool on);
bool relay_get_state(void);

#endif
