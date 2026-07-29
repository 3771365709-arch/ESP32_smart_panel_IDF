#include <stdio.h>
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "relay.h"
#include "pwm_led.h"
#include "sensor.h"
#include "wifi_connect.h"
#include "mqtt_client.h"
#include "ui_manager.h"

// 数据上报任务：定时上报温湿度到MQTT
static void data_report_task(void *pv) {
    while (1) {
        float temp = sensor_get_temperature();
        float humi = sensor_get_humidity();

        // 更新UI显示
        ui_update_sensor(temp, humi);

        // MQTT上报（已连接才上报）
        mqtt_publish_sensor(temp, humi);

        vTaskDelay(pdMS_TO_TICKS(5000)); // 5秒上报一次
    }
}

void app_main(void) {
    printf("=== ESP32智能控制面板启动 ===\n");

    // 1. 初始化NVS（WiFi/系统参数存储）
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    // 2. 硬件驱动初始化
    relay_init();
    pwm_led_init();
    sensor_init();

    // 3. 网络初始化
    wifi_init();
    mqtt_start();

    // 4. UI初始化（LVGL + 显示驱动 + 所有页面）
    ui_init();

    // 5. 启动业务任务
    xTaskCreate(sensor_task, "sensor_task", 2048, NULL, 4, NULL);
    xTaskCreate(data_report_task, "data_report", 4096, NULL, 3, NULL);

    printf("系统初始化完成，进入LVGL主循环\n");

    // 6. LVGL主循环（必须放在app_main最后，持续运行）
    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
