#include "pwm_led.h"

static int current_brightness = 80;

void pwm_led_init(void) {
    ledc_timer_config_t timer_cfg = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQ,
        .duty_resolution = LEDC_RESOLUTION,
    };
    ledc_timer_config(&timer_cfg);

    ledc_channel_config_t ch_cfg = {
        .gpio_num = LEDC_GPIO,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&ch_cfg);

    pwm_set_brightness(current_brightness);
}

void pwm_set_brightness(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    current_brightness = percent;

    uint32_t duty = (uint32_t)(percent * 8191 / 100);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

int pwm_get_brightness(void) {
    return current_brightness;
}
