#include "scr_setting.h"
#include "ui_manager.h"
#include "ota_update.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 返回主页
static void goto_main_cb(lv_event_t *e) {
    (void)e;
    extern void ui_switch_to(int id);
    ui_switch_to(0); // SCREEN_MAIN
}

// 跳转到关于页
static void goto_about_cb(lv_event_t *e) {
    (void)e;
    extern void ui_switch_to(int id);
    ui_switch_to(2); // SCREEN_ABOUT
}

// OTA升级按钮回调
static void ota_btn_cb(lv_event_t *e) {
    (void)e;
    // 在独立任务中执行OTA，避免阻塞UI
    xTaskCreate(ota_update_task, "ota_task", 8192, NULL, 5, NULL);
}

lv_obj_t *scr_setting_create(void) {
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a2e), 0);

    // 标题
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "系统设置");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // 设置项列表容器
    lv_obj_t *list = lv_obj_create(scr);
    lv_obj_set_size(list, SCREEN_WIDTH - 20, 180);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 55);
    lv_obj_set_style_bg_color(list, lv_color_hex(0x0f3460), 0);
    lv_obj_set_style_border_width(list, 0, 0);
    lv_obj_set_style_pad_all(list, 10, 0);

    // OTA升级按钮
    lv_obj_t *btn_ota = lv_btn_create(list);
    lv_obj_set_size(btn_ota, SCREEN_WIDTH - 60, 40);
    lv_obj_align(btn_ota, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_bg_color(btn_ota, lv_color_hex(0x533483), 0);
    lv_obj_add_event_cb(btn_ota, ota_btn_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label_ota = lv_label_create(btn_ota);
    lv_label_set_text(label_ota, "检查固件更新");
    lv_obj_set_style_text_color(label_ota, lv_color_white(), 0);
    lv_obj_center(label_ota);

    // 关于按钮
    lv_obj_t *btn_about = lv_btn_create(list);
    lv_obj_set_size(btn_about, SCREEN_WIDTH - 60, 40);
    lv_obj_align(btn_about, LV_ALIGN_TOP_MID, 0, 65);
    lv_obj_set_style_bg_color(btn_about, lv_color_hex(0x533483), 0);
    lv_obj_add_event_cb(btn_about, goto_about_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label_about = lv_label_create(btn_about);
    lv_label_set_text(label_about, "关于设备");
    lv_obj_set_style_text_color(label_about, lv_color_white(), 0);
    lv_obj_center(label_about);

    // WiFi设置提示
    lv_obj_t *label_wifi_tip = lv_label_create(list);
    lv_label_set_text(label_wifi_tip, "WiFi配置: 修改config_local.h");
    lv_obj_set_style_text_color(label_wifi_tip, lv_color_hex(0xaaaaaa), 0);
    lv_obj_align(label_wifi_tip, LV_ALIGN_TOP_MID, 0, 125);

    // 返回按钮
    lv_obj_t *btn_back = lv_btn_create(scr);
    lv_obj_set_size(btn_back, 80, 36);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0x533483), 0);
    lv_obj_add_event_cb(btn_back, goto_main_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "返回");
    lv_obj_set_style_text_color(label_back, lv_color_white(), 0);
    lv_obj_center(label_back);

    return scr;
}
