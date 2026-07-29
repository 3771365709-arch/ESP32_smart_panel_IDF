#ifndef _OTA_UPDATE_H_
#define _OTA_UPDATE_H_

#include "esp_err.h"

esp_err_t ota_update(const char *url);
void ota_update_task(void *pvParameters);

#endif
