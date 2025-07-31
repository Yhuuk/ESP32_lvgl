#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// 屏幕对象
TFT_eSPI tft = TFT_eSPI(TFT_WIDTH,TFT_HEIGHT); 

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
// void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
//     if (ts.touched()) {
//         TS_Point p = ts.getPoint();
//         data->state = LV_INDEV_STATE_PRESSED;
//         data->point.x = p.x;
//         data->point.y = p.y;
//     } else {
//         data->state = LV_INDEV_STATE_RELEASED;
//     }
// }

void setup() {
    Serial.begin(115200);

    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_WHITE);

    lv_init();

    // 创建并注册 LVGL 显示驱动
    static lv_display_t * disp;
    disp = lv_display_create(240, 320);

    // 分配显示缓冲区（推荐用一行或多行缓冲，节省内存）
    // static lv_color_t buf1[240 * 10]; // 10行缓冲
    // lv_display_set_buffers(disp, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // 1. 分配缓冲区
static lv_color_t buf1[240 * 10];  // 一次绘制10行
static lv_draw_buf_t draw_buf;

// 2. 初始化缓冲区
lv_draw_buf_init(&draw_buf,
                 240,                   // 屏幕宽度
                 320,                   // 屏幕高度
                 LV_COLOR_FORMAT_RGB565,  // 或 LV_COLOR_FORMAT_NATIVE
                 0,                     // flags（暂时设为0）
                 buf1,                  // 缓冲区指针
                 sizeof(buf1));         // 缓冲区大小（字节）

// 3. 设置显示缓冲
lv_display_set_draw_buffers(disp, &draw_buf, NULL);
    lv_display_set_flush_cb(disp, my_disp_flush);

    // lv_display_set_flush_cb(disp, my_disp_flush);

    // 创建并注册输入设备（指针类型）
    // static lv_indev_t * indev;
    // indev = lv_indev_create();
    // lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    // lv_indev_set_read_cb(indev, my_touchpad_read);

    lv_obj_t *label = lv_label_create(lv_scr_act()); // 创建在当前屏幕上    
    lv_label_set_text(label, "Hello, LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
}

void loop() {
    lv_timer_handler();  // LVGL 处理器
    delay(5);            // 最少延迟
}

