#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"

TFT_eSPI tft = TFT_eSPI();  // TFT 驱动实例

// #define SCREEN_HOR_RES 320
// #define SCREEN_VER_RES 240

#define SCREEN_HOR_RES 240
#define SCREEN_VER_RES 320

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

    // 初始化 SquareLine UI
    ui_init();
}

void loop() {
    lv_timer_handler();  // 处理 LVGL 任务
    delay(5);
}
