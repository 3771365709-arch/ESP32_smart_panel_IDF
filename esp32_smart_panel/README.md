# ESP32 智能控制面板工程

基于 ESP-IDF + LVGL 的智能控制面板完整工程模板，支持触摸屏、WiFi、MQTT、OTA升级。

## 功能特性

- 🎨 **LVGL图形界面**：三页面切换（主页/设置/关于），卡片式UI
- 💡 **继电器控制**：GPIO控制灯光开关
- ☀️ **PWM调光**：LEDC外设实现亮度调节
- 🌡️ **温湿度监测**：模拟数据，可替换为真实传感器
- 📶 **WiFi联网**：自动重连，STA模式
- 📡 **MQTT通信**：远程控制 + 数据上报
- ⬆️ **OTA空中升级**：HTTP下载固件，双分区切换
- 📱 **触摸交互**：支持电容/电阻触摸屏

## 硬件要求

- ESP32开发板（WROOM-32）
- ST7789 SPI彩屏（240×320，2.4寸）
- 触摸屏（FT6236电容屏 或 XPT2046电阻屏）
- 继电器模块（可选）
- LED + 220Ω电阻（可选，PWM调光测试用）

### 接线参考

| 模块 | ESP32引脚 |
|------|----------|
| LCD_SCK | GPIO18 |
| LCD_MOSI | GPIO23 |
| LCD_CS | GPIO5 |
| LCD_DC | GPIO2 |
| LCD_RST | GPIO4 |
| LCD_BL | GPIO15 |
| 继电器 | GPIO26 |
| PWM LED | GPIO27 |
| 触摸I2C_SDA | GPIO21 |
| 触摸I2C_SCL | GPIO22 |

## 快速开始

### 1. 环境准备

已安装 ESP-IDF v5.x 开发环境，`idf.py` 命令可用。

### 2. 配置本地参数

```bash
cd main
cp config_local_template.h config_local.h
# 编辑 config_local.h，填入你的WiFi名称和密码
```

### 3. 安装LVGL组件

```bash
# 在项目根目录执行
idf.py add-dependency "lvgl/lvgl^8.4.0"
```

将 `lvgl_esp32_drivers` 组件放入 `components/` 目录：
```bash
cd components
git clone https://github.com/lvgl/lvgl_esp32_drivers.git
```

### 4. 菜单配置

```bash
idf.py menuconfig
```

进入 `Component config → LVGL ESP32 Drivers`：
- 选择屏幕控制器：ST7789
- 配置SPI引脚（与实际接线一致）
- 选择触摸控制器：FT6X36 或 XPT2046
- 配置触摸引脚

进入 `Component config → LVGL configuration`：
- LV_COLOR_DEPTH = 16
- LV_HOR_RES_MAX = 240
- LV_VER_RES_MAX = 320
- 勾选 LV_TICK_CUSTOM

进入 `Partition Table`：
- 选择 Custom partition table CSV
- Partition table CSV = partitions.csv

### 5. 编译烧录

```bash
# 设置目标芯片
idf.py set-target esp32

# 编译
idf.py build

# 烧录 + 串口监控
idf.py -p /dev/ttyUSB0 flash monitor
```

## 工程目录结构

```
esp32_smart_panel/
├── CMakeLists.txt          # 顶层CMake
├── partitions.csv          # Flash分区表（OTA双分区）
├── .gitignore              # Git忽略规则
├── README.md               # 本文件
│
├── main/                   # 主程序
│   ├── CMakeLists.txt
│   ├── app_main.c          # 入口函数
│   ├── wifi_connect.c/h    # WiFi连接管理
│   ├── mqtt_client.c/h     # MQTT客户端
│   ├── ota_update.c/h      # OTA空中升级
│   ├── config_local_template.h  # 本地配置模板
│   │
│   ├── hardware/           # 硬件驱动层
│   │   ├── relay.c/h       # 继电器
│   │   ├── pwm_led.c/h     # PWM调光
│   │   └── sensor.c/h      # 温湿度传感器
│   │
│   └── ui/                 # UI界面层
│       ├── ui_manager.c/h  # 页面管理器
│       ├── scr_main.c/h    # 主控制面板
│       ├── scr_setting.c/h # 设置页面
│       └── scr_about.c/h   # 关于页面
│
└── components/             # 第三方组件
    ├── lvgl/               # LVGL图形库
    └── lvgl_esp32_drivers/ # ESP32显示触摸驱动
```

## 架构说明

采用分层设计：

```
应用层 (app_main)
    ├── 业务任务调度
    └── 初始化流程
UI层 (ui/)
    ├── 多页面管理
    ├── 控件事件回调
    └── 数据显示更新
系统层 (wifi/mqtt/ota)
    ├── 网络通信
    └── 固件升级
硬件层 (hardware/)
    ├── GPIO/PWM
    └── 传感器驱动
硬件驱动 (components/)
    ├── LVGL
    └── 显示触摸驱动
```

## MQTT主题

| 主题 | 方向 | 说明 |
|------|------|------|
| `device/sensor/data` | 设备→云端 | 上报温湿度JSON数据 |
| `device/control/light` | 云端→设备 | 控制继电器："1"开 "0"关 |

数据格式示例：
```json
{"temp":25.3, "humi":65}
```

## 版本管理

### 固件版本号
在 `main/ui/scr_about.c` 中修改 `FIRMWARE_VERSION` 宏定义。

### Git提交规范
```
feat: 新增功能
fix: 修复bug
refactor: 重构代码
docs: 文档更新
perf: 性能优化
```

## 常见问题

### 编译报错找不到lvgl.h
确认components目录下有lvgl组件，或使用 `idf.py add-dependency` 安装。

### 屏幕白屏不显示
1. 检查SPI接线是否正确
2. 确认menuconfig中引脚配置与实际一致
3. 检查背光引脚是否接高电平

### 触摸没反应
1. 确认触摸芯片型号选择正确
2. 检查I2C/SPI接线
3. 电阻屏需先做校准

### WiFi连不上
1. 确认是2.4G WiFi（ESP32不支持5G）
2. 检查config_local.h中SSID和密码
3. 查看串口日志排查错误

## 后续扩展方向

- [ ] 接入真实温湿度传感器（SHT30/DHT22）
- [ ] 添加定时任务功能
- [ ] 接入阿里云/腾讯云IoT平台
- [ ] 增加数据历史曲线图表
- [ ] 低功耗熄屏唤醒
- [ ] 看门狗异常自动恢复

## 许可证

仅供学习参考使用。
