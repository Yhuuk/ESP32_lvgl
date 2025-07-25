#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// 屏幕对象
TFT_eSPI tft = TFT_eSPI(); 

// 触摸对象
#define CS_PIN  33  // XPT2046 的 CS 引脚，按你的实际接法修改
XPT2046_Touchscreen ts(CS_PIN);

// LVGL 显示刷新回调
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors((uint16_t *)color_p, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();
    lv_display_flush_ready(disp);
}

// Touch 回调（v9.0 中必须是 void 类型）
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
    if (ts.touched()) {
        TS_Point p = ts.getPoint();
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = p.x;
        data->point.y = p.y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void setup() {
    Serial.begin(115200);

    // 初始化屏幕
    tft.begin();
    tft.setRotation(1); // 根据屏幕方向调整
    ts.begin();
    ts.setRotation(1);  // 根据触摸方向调整

    // 初始化 LVGL
    lv_init();

    // 创建并注册 LVGL 显示驱动
    static lv_display_t * disp;
    disp = lv_display_create(240, 320);  // 你的分辨率
    lv_display_set_flush_cb(disp, my_disp_flush);

    // 创建并注册输入设备（指针类型）
    static lv_indev_t * indev;
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);
}

void loop() {
    lv_timer_handler();  // LVGL 处理器
    delay(5);            // 最少延迟
}
