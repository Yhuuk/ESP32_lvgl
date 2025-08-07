#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "ui.h"

TFT_eSPI tft = TFT_eSPI();  // TFT 驱动实例

// #define SCREEN_HOR_RES 320
// #define SCREEN_VER_RES 240

#define SCREEN_HOR_RES 240
#define SCREEN_VER_RES 320

SPIClass mySPI(HSPI);  // 或者 HSPI，看你连接的是哪一组 SPI
#define DOUT 39  /* Data out pin (T_DO) of touch screen */
#define DIN  32  /* Data in pin (T_DIN) of touch screen */
#define DCS  33  /* Chip select pin (T_CS) of touch screen */
#define DCLK 25  /* Clock pin (T_CLK) of touch screen */

// #define TOUCH_CS 33    // 你的 XPT2046 的 CS 引脚
#define TOUCH_IRQ 255  // 没接 IRQ 就设 255
#define TOUCH_ROTATION 0

XPT2046_Touchscreen ts(DCS, TOUCH_IRQ);  // ✅ 创建触摸屏对象

/* 显存缓存（2行） */
static lv_color_t buf[SCREEN_HOR_RES * 40];
static lv_display_t *disp;

void my_disp_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map) {   // 刷新回调函数
    // 将 LVGL 渲染的像素数据 px_map 写入 TFT 显示屏，px_map指向 LVGL 渲染出来的 RGB 或 RGB565 像素数据
    // area 定义了需要刷新的区域    
    uint16_t *color_map = (uint16_t *)px_map;
    tft.startWrite();  // 开始写入数据到 TFT
    tft.setAddrWindow(area->x1, area->y1,   
                      area->x2 - area->x1 + 1,
                      area->y2 - area->y1 + 1);  // 设置写入窗口 成员 x1, y1, x2, y2 表示左上角到右下角的坐标
    // 将像素数据写入 TFT 显示屏
    tft.pushColors(color_map, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();  // 结束写入数据到 TFT

    lv_display_flush_ready(disp_drv); // 通知 LVGL 刷新已完成
}


void my_touchpad_read(lv_indev_t *indev_driver, lv_indev_data_t *data) {
    if (ts.touched()) {
        TS_Point p = ts.getPoint();
        Serial.print("Touch:ok!");
        uint16_t x = p.x;
        uint16_t y = p.y;

        // 简单映射，根据你实际坐标范围微调
        data->point.x = map(p.x, 200, 3900, 0, SCREEN_HOR_RES);
        data->point.y = map(p.y, 200, 3900, 0, SCREEN_VER_RES);
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
   
}


hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() { // 定时器中断回调函数
    // 每1ms 调用一次 lv_tick_inc
    // 这里使用 IRAM_ATTR 确保函数在 IRAM 中执行，避免延迟
  lv_tick_inc(1); // 调用 LVGL 的 tick 增量函数，通知 LVGL 过去了多少毫秒
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    tft.begin();
    tft.setRotation(0);  // 根据实际调整 数值0~3
    tft.fillScreen(TFT_BLACK); // 清屏（只执行一次，后续由LVGL控制屏幕内容）

    mySPI.begin(DCLK, DIN, DOUT, DCS);  // 初始化 SPI，DCLK, DIN, DOUT, DCS 分别对应触摸屏的时钟、数据输入、数据输出和片选引脚
    mySPI.setFrequency(2000000); // 设置 SPI 频率为 2MHz
    ts.begin(mySPI);  // 初始化触摸屏，传入 SPI 对象
    ts.setRotation(TOUCH_ROTATION);  // 根据实际调整 数值0~3

    lv_init();  // 初始化 LVGL

      // 设置 LVGL 时钟，配置lvgl的硬件定时器，每1ms 调用一次 lv_tick_inc
    timer = timerBegin(0, 80, true);  // 80分频，1MHz时钟，使用定时器0（总共4个定时器），true表示计数向上
    timerAttachInterrupt(timer, &onTimer, true);  // true表示在上升沿触发中断
    // 设置定时器报警时间为1000us（1ms），并设置自动重载
    // 这样每1ms会触发一次中断，调用 onTimer 函数
    timerAlarmWrite(timer, 1000, true);  // 1000us = 1ms
    timerAlarmEnable(timer);  // 启用定时器报警
    // 创建 LVGL 显示驱动
    disp = lv_display_create(SCREEN_HOR_RES, SCREEN_VER_RES);    // 创建显示驱动
    lv_display_set_flush_cb(disp, my_disp_flush);   // 设置刷新回调函数
    lv_display_set_buffers(disp, buf, nullptr, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL); // 设置双缓冲，使用部分渲染模式，只刷新有变化的区域（更高效）

   // 注册触摸输入设备
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);

    // 初始化 SquareLine UI
    ui_init();
}

void loop() {
    lv_timer_handler();  // 处理 LVGL 任务
    delay(5);
}
