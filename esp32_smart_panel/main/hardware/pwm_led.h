#ifndef _PWM_LED_H_
#define _PWM_LED_H_

#include "driver/ledc.h"

#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL    LEDC_CHANNEL_0
#define LEDC_GPIO       GPIO_NUM_27
#define LEDC_FREQ       5000
#define LEDC_RESOLUTION LEDC_TIMER_13_BIT

void pwm_led_init(void);
void pwm_set_brightness(int percent);
int pwm_get_brightness(void);

#endif
