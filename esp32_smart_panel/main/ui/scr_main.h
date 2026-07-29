#ifndef _SCR_MAIN_H_
#define _SCR_MAIN_H_

#include "lvgl.h"

lv_obj_t *scr_main_create(void);
void scr_main_update_sensor(float temp, float humi);

#endif
