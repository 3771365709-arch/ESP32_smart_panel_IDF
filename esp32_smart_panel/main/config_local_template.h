// 本地配置模板
// 使用方法：复制本文件为 config_local.h，填入你的真实配置
// config_local.h 已加入 .gitignore，不会提交到代码仓库

#ifndef _CONFIG_LOCAL_H_
#define _CONFIG_LOCAL_H_

// WiFi配置
#define WIFI_SSID     "你的WiFi名称"
#define WIFI_PASSWORD "你的WiFi密码"

// MQTT配置（可选修改）
#define MQTT_BROKER_URL "mqtt://mqtt.eclipseprojects.io"

// OTA升级地址（可选修改）
#define OTA_UPDATE_URL "http://your-server.com/firmware/latest.bin"

#endif
