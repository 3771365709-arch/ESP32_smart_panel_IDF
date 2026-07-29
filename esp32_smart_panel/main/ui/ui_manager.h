#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "lvgl.h"

#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   320

typedef enum {
    SCREEN_MAIN = 0,
    SCREEN_SETTING,
    SCREEN_ABOUT,
} screen_id_t;

void ui_init(void);
void ui_switch_to(screen_id_t id);
void ui_update_sensor(float temp, float humi);

#endif
