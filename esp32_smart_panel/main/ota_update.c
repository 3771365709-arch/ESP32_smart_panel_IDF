#include "ota_update.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "OTA";

// 默认OTA升级地址（可在设置页修改）
#define OTA_URL_DEFAULT "http://your-server.com/firmware/smart_panel_latest.bin"

esp_err_t ota_update(const char *url) {
    ESP_LOGI(TAG, "开始OTA升级: %s", url);

    esp_http_client_config_t config = {
        .url = url,
        .timeout_ms = 60000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "HTTP客户端初始化失败");
        return ESP_FAIL;
    }

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP连接失败: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return err;
    }

    int content_len = esp_http_client_fetch_headers(client);
    if (content_len <= 0) {
        ESP_LOGE(TAG, "获取固件大小失败");
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    const esp_partition_t *update_part = esp_ota_get_next_update_partition(NULL);
    if (update_part == NULL) {
        ESP_LOGE(TAG, "找不到OTA分区");
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    esp_ota_handle_t update_handle = 0;
    err = esp_ota_begin(update_part, OTA_SIZE_UNKNOWN, &update_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "OTA开始失败: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return err;
    }

    char buf[1024];
    int total_read = 0;
    while (total_read < content_len) {
        int read_len = esp_http_client_read(client, buf, sizeof(buf));
        if (read_len <= 0) break;

        err = esp_ota_write(update_handle, buf, read_len);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "OTA写入失败");
            break;
        }
        total_read += read_len;
    }

    esp_ota_end(update_handle);
    esp_http_client_cleanup(client);

    if (total_read != content_len) {
        ESP_LOGE(TAG, "固件下载不完整: %d/%d", total_read, content_len);
        return ESP_FAIL;
    }

    err = esp_ota_set_boot_partition(update_part);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "设置启动分区失败");
        return err;
    }

    ESP_LOGI(TAG, "升级成功，3秒后重启");
    vTaskDelay(pdMS_TO_TICKS(3000));
    esp_restart();

    return ESP_OK;
}

void ota_update_task(void *pvParameters) {
    const char *url = (pvParameters != NULL) ? (const char*)pvParameters : OTA_URL_DEFAULT;
    ota_update(url);
    vTaskDelete(NULL);
}
