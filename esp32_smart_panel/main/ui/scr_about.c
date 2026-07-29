#include "scr_about.h"
#include "ui_manager.h"

#define FIRMWARE_VERSION "v1.0.0"

// 返回设置页
static void goto_setting_cb(lv_event_t *e) {
    (void)e;
    extern void ui_switch_to(int id);
    ui_switch_to(1); // SCREEN_SETTING
}

lv_obj_t *scr_about_create(void) {
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a2e), 0);

    // 标题
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "关于");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // 信息卡片
    lv_obj_t *card = lv_obj_create(scr);
    lv_obj_set_size(card, SCREEN_WIDTH - 20, 180);
    lv_obj_align(card, LV_ALIGN_TOP_MID, 0, 55);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x0f3460), 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_pad_all(card, 15, 0);

    // 设备名称
    lv_obj_t *label_name = lv_label_create(card);
    lv_label_set_text(label_name, "设备: ESP32智能面板");
    lv_obj_set_style_text_color(label_name, lv_color_white(), 0);
    lv_obj_align(label_name, LV_ALIGN_TOP_LEFT, 0, 10);

    // 固件版本
    lv_obj_t *label_ver = lv_label_create(card);
    lv_label_set_text_fmt(label_ver, "固件版本: %s", FIRMWARE_VERSION);
    lv_obj_set_style_text_color(label_ver, lv_color_white(), 0);
    lv_obj_align(label_ver, LV_ALIGN_TOP_LEFT, 0, 35);

    // LVGL版本
    lv_obj_t *label_lvgl = lv_label_create(card);
    lv_label_set_text_fmt(label_lvgl, "LVGL版本: %d.%d.%d",
                          LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);
    lv_obj_set_style_text_color(label_lvgl, lv_color_white(), 0);
    lv_obj_align(label_lvgl, LV_ALIGN_TOP_LEFT, 0, 60);

    // 芯片
    lv_obj_t *label_chip = lv_label_create(card);
    lv_label_set_text(label_chip, "芯片: ESP32 Xtensa");
    lv_obj_set_style_text_color(label_chip, lv_color_white(), 0);
    lv_obj_align(label_chip, LV_ALIGN_TOP_LEFT, 0, 85);

    // 分隔线
    lv_obj_t *line = lv_obj_create(card);
    lv_obj_set_size(line, SCREEN_WIDTH - 70, 1);
    lv_obj_align(line, LV_ALIGN_TOP_LEFT, 0, 115);
    lv_obj_set_style_bg_color(line, lv_color_hex(0x555555), 0);
    lv_obj_set_style_border_width(line, 0, 0);

    // 版权
    lv_obj_t *label_copy = lv_label_create(card);
    lv_label_set_text(label_copy, "Smart Panel Demo");
    lv_obj_set_style_text_color(label_copy, lv_color_hex(0xaaaaaa), 0);
    lv_obj_align(label_copy, LV_ALIGN_TOP_LEFT, 0, 130);

    // 返回按钮
    lv_obj_t *btn_back = lv_btn_create(scr);
    lv_obj_set_size(btn_back, 80, 36);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0x533483), 0);
    lv_obj_add_event_cb(btn_back, goto_setting_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "返回");
    lv_obj_set_style_text_color(label_back, lv_color_white(), 0);
    lv_obj_center(label_back);

    return scr;
}
