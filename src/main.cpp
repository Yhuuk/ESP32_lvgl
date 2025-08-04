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

void my_disp_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map) {
    uint16_t *color_map = (uint16_t *)px_map;
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1,
                      area->x2 - area->x1 + 1,
                      area->y2 - area->y1 + 1);
    tft.pushColors(color_map, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();

    lv_display_flush_ready(disp_drv);
}

hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
  lv_tick_inc(1);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    tft.begin();
    tft.setRotation(0);  // 根据实际调整
    tft.fillScreen(TFT_BLACK);

    lv_init();

      // 设置 LVGL 时钟
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000, true);  // 1000us = 1ms
    timerAlarmEnable(timer);
    // 创建 LVGL 显示驱动
    disp = lv_display_create(SCREEN_HOR_RES, SCREEN_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf, nullptr, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // 初始化 SquareLine UI
    ui_init();
}

void loop() {
    lv_timer_handler();  // 处理 LVGL 任务
    delay(5);
}
