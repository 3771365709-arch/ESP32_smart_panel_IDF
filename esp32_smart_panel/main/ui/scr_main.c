#include "scr_main.h"
#include "ui_manager.h"
#include "relay.h"
#include "pwm_led.h"
#include "wifi_connect.h"

static lv_obj_t *label_temp = NULL;
static lv_obj_t *label_humi = NULL;
static lv_obj_t *sw_light = NULL;
static lv_obj_t *slider_bright = NULL;
static lv_obj_t *label_bright_val = NULL;
static lv_obj_t *label_wifi = NULL;

// 灯光开关回调
static void light_switch_cb(lv_event_t *e) {
    bool state = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    relay_set(state);
}

// 亮度滑块回调
static void brightness_cb(lv_event_t *e) {
    int val = lv_slider_get_value(lv_event_get_target(e));
    lv_label_set_text_fmt(label_bright_val, "%d%%", val);
    pwm_set_brightness(val);
}

// 跳转到设置页
static void goto_setting_cb(lv_event_t *e) {
    (void)e;
    extern void ui_switch_to(int id);
    ui_switch_to(1); // SCREEN_SETTING
}

lv_obj_t *scr_main_create(void) {
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a2e), 0);

    // ---- 顶部状态栏 ----
    lv_obj_t *status_bar = lv_obj_create(scr);
    lv_obj_set_size(status_bar, SCREEN_WIDTH, 28);
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_radius(status_bar, 0, 0);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x16213e), 0);
    lv_obj_set_style_border_width(status_bar, 0, 0);
    lv_obj_set_style_pad_all(status_bar, 0, 0);

    label_wifi = lv_label_create(status_bar);
    lv_label_set_text(label_wifi, "WiFi: --");
    lv_obj_set_style_text_color(label_wifi, lv_color_white(), 0);
    lv_obj_align(label_wifi, LV_ALIGN_LEFT_MID, 8, 0);

    // ---- 标题 ----
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "智能控制面板");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 38);

    // ---- 灯光控制卡片 ----
    lv_obj_t *panel_light = lv_obj_create(scr);
    lv_obj_set_size(panel_light, SCREEN_WIDTH - 20, 60);
    lv_obj_align(panel_light, LV_ALIGN_TOP_MID, 0, 65);
    lv_obj_set_style_bg_color(panel_light, lv_color_hex(0x0f3460), 0);
    lv_obj_set_style_border_width(panel_light, 0, 0);

    lv_obj_t *light_title = lv_label_create(panel_light);
    lv_label_set_text(light_title, "灯光开关");
    lv_obj_set_style_text_color(light_title, lv_color_white(), 0);
    lv_obj_align(light_title, LV_ALIGN_LEFT_MID, 12, 0);

    sw_light = lv_switch_create(panel_light);
    lv_obj_set_size(sw_light, 48, 24);
    lv_obj_align(sw_light, LV_ALIGN_RIGHT_MID, -12, 0);
    lv_obj_add_event_cb(sw_light, light_switch_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // ---- 亮度调节卡片 ----
    lv_obj_t *panel_bright = lv_obj_create(scr);
    lv_obj_set_size(panel_bright, SCREEN_WIDTH - 20, 55);
    lv_obj_align(panel_bright, LV_ALIGN_TOP_MID, 0, 135);
    lv_obj_set_style_bg_color(panel_bright, lv_color_hex(0x0f3460), 0);
    lv_obj_set_style_border_width(panel_bright, 0, 0);

    lv_obj_t *bright_title = lv_label_create(panel_bright);
    lv_label_set_text(bright_title, "亮度调节");
    lv_obj_set_style_text_color(bright_title, lv_color_white(), 0);
    lv_obj_align(bright_title, LV_ALIGN_TOP_LEFT, 12, 6);

    slider_bright = lv_slider_create(panel_bright);
    lv_obj_set_width(slider_bright, SCREEN_WIDTH - 80);
    lv_obj_align(slider_bright, LV_ALIGN_BOTTOM_LEFT, 12, -6);
    lv_slider_set_value(slider_bright, 80, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_bright, brightness_cb, LV_EVENT_VALUE_CHANGED, NULL);

    label_bright_val = lv_label_create(panel_bright);
    lv_label_set_text(label_bright_val, "80%");
    lv_obj_set_style_text_color(label_bright_val, lv_color_white(), 0);
    lv_obj_align(label_bright_val, LV_ALIGN_BOTTOM_RIGHT, -12, -6);

    // ---- 温湿度卡片 ----
    lv_obj_t *panel_th = lv_obj_create(scr);
    lv_obj_set_size(panel_th, SCREEN_WIDTH - 20, 55);
    lv_obj_align(panel_th, LV_ALIGN_TOP_MID, 0, 200);
    lv_obj_set_style_bg_color(panel_th, lv_color_hex(0xe94560), 0);
    lv_obj_set_style_border_width(panel_th, 0, 0);

    label_temp = lv_label_create(panel_th);
    lv_label_set_text(label_temp, "温度: --.- C");
    lv_obj_set_style_text_color(label_temp, lv_color_white(), 0);
    lv_obj_align(label_temp, LV_ALIGN_LEFT_MID, 15, 0);

    label_humi = lv_label_create(panel_th);
    lv_label_set_text(label_humi, "湿度: --%");
    lv_obj_set_style_text_color(label_humi, lv_color_white(), 0);
    lv_obj_align(label_humi, LV_ALIGN_RIGHT_MID, -15, 0);

    // ---- 设置按钮 ----
    lv_obj_t *btn_setting = lv_btn_create(scr);
    lv_obj_set_size(btn_setting, 80, 36);
    lv_obj_align(btn_setting, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_set_style_bg_color(btn_setting, lv_color_hex(0x533483), 0);
    lv_obj_add_event_cb(btn_setting, goto_setting_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label_setting = lv_label_create(btn_setting);
    lv_label_set_text(label_setting, "设置");
    lv_obj_set_style_text_color(label_setting, lv_color_white(), 0);
    lv_obj_center(label_setting);

    return scr;
}

void scr_main_update_sensor(float temp, float humi) {
    if (label_temp) {
        lv_label_set_text_fmt(label_temp, "温度: %.1f C", temp);
    }
    if (label_humi) {
        lv_label_set_text_fmt(label_humi, "湿度: %.0f%%", humi);
    }
    if (label_wifi) {
        lv_label_set_text(label_wifi, wifi_is_connected() ? "WiFi: 已连" : "WiFi: 断开");
    }
}
