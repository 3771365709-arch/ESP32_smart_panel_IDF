#include "sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdlib.h>

static float temperature = 25.0f;
static float humidity = 65.0f;

void sensor_init(void) {
    // 模拟传感器初始化
    // 实际项目中在这里初始化I2C + SHT30/DHT22等
    temperature = 25.0f;
    humidity = 65.0f;
}

float sensor_get_temperature(void) {
    return temperature;
}

float sensor_get_humidity(void) {
    return humidity;
}

void sensor_task(void *pvParameters) {
    while (1) {
        // 模拟数据波动（实际项目中替换为真实传感器读取）
        float delta_t = (float)(rand() % 100 - 50) / 100.0f;
        float delta_h = (float)(rand() % 100 - 50) / 100.0f;

        temperature += delta_t;
        humidity += delta_h;

        // 范围限制
        if (temperature < 10.0f) temperature = 10.0f;
        if (temperature > 40.0f) temperature = 40.0f;
        if (humidity < 30.0f) humidity = 30.0f;
        if (humidity > 90.0f) humidity = 90.0f;

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
