#include "ui_manager.h"
#include "scr_main.h"
#include "scr_setting.h"
#include "scr_about.h"
#include "lvgl_helpers.h"
#include "esp_timer.h"

static lv_obj_t *scr_main_obj = NULL;
static lv_obj_t *scr_setting_obj = NULL;
static lv_obj_t *scr_about_obj = NULL;

static void lv_tick_task(void *arg) {
    (void)arg;
    lv_tick_inc(10);
}

void ui_init(void) {
    // 1. 初始化LVGL
    lv_init();

    // 2. 初始化显示驱动
    lvgl_driver_init();

    // 3. 显示缓冲区（双缓冲）
    static lv_color_t buf1[SCREEN_WIDTH * 20];
    static lv_color_t buf2[SCREEN_WIDTH * 20];
    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, SCREEN_WIDTH * 20);

    // 4. 注册显示设备
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    lv_disp_drv_register(&disp_drv);

    // 5. 注册触摸输入设备
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_driver_read;
    lv_indev_drv_register(&indev_drv);

    // 6. LVGL心跳定时器（10ms）
    const esp_timer_create_args_t timer_args = {
        .callback = &lv_tick_task,
        .name = "lv_tick_timer"
    };
    esp_timer_handle_t timer;
    esp_timer_create(&timer_args, &timer);
    esp_timer_start_periodic(timer, 10 * 1000);

    // 7. 创建所有页面
    scr_main_obj = scr_main_create();
    scr_setting_obj = scr_setting_create();
    scr_about_obj = scr_about_create();

    // 8. 默认显示主页
    lv_scr_load(scr_main_obj);
}

void ui_switch_to(screen_id_t id) {
    lv_obj_t *target = NULL;
    lv_scr_load_anim_t anim = LV_SCR_LOAD_ANIM_MOVE_LEFT;

    switch (id) {
        case SCREEN_MAIN:
            target = scr_main_obj;
            anim = LV_SCR_LOAD_ANIM_MOVE_RIGHT;
            break;
        case SCREEN_SETTING:
            target = scr_setting_obj;
            anim = LV_SCR_LOAD_ANIM_MOVE_LEFT;
            break;
        case SCREEN_ABOUT:
            target = scr_about_obj;
            anim = LV_SCR_LOAD_ANIM_MOVE_LEFT;
            break;
    }

    if (target) {
        lv_scr_load_anim(target, anim, 200, 0, false);
    }
}

void ui_update_sensor(float temp, float humi) {
    scr_main_update_sensor(temp, humi);
}
