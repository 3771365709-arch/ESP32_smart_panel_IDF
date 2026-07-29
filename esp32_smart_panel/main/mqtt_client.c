#include "mqtt_client.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include "relay.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "MQTT";
static esp_mqtt_client_handle_t client = NULL;
static bool mqtt_connected = false;

// 主题定义
#define TOPIC_SENSOR    "device/sensor/data"
#define TOPIC_CONTROL   "device/control/light"

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT已连接");
            mqtt_connected = true;
            esp_mqtt_client_subscribe(client, TOPIC_CONTROL, 0);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT断开连接");
            mqtt_connected = false;
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "收到指令: topic=%.*s data=%.*s",
                     event->topic_len, event->topic,
                     event->data_len, event->data);

            // 简单控制：收到"1"开继电器，收到"0"关
            if (event->data_len >= 1) {
                if (event->data[0] == '1') {
                    relay_set(true);
                } else if (event->data[0] == '0') {
                    relay_set(false);
                }
            }
            break;

        default:
            break;
    }
}

void mqtt_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://mqtt.eclipseprojects.io",
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

bool mqtt_is_connected(void) {
    return mqtt_connected;
}

void mqtt_publish_sensor(float temp, float humi) {
    if (!mqtt_connected || !client) return;

    char payload[64];
    snprintf(payload, sizeof(payload), "{\"temp\":%.1f, \"humi\":%.0f}", temp, humi);
    esp_mqtt_client_publish(client, TOPIC_SENSOR, payload, 0, 1, 0);
}
