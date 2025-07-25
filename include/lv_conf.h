/**
 * lv_conf.h - Custom configuration for LVGL
 * For ESP32 + 320x240 TFT (ILI9341) + Touch (XPT2046)
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/*====================
   LVGL Configuration
 *====================*/

#define LV_HOR_RES_MAX          (320)
#define LV_VER_RES_MAX          (240)
#define LV_COLOR_DEPTH          16
#define LV_COLOR_16_SWAP        1      // Swap bytes for 16-bit colors (little-endian MCUs like ESP32)

#define LV_TICK_CUSTOM          1
#define LV_TICK_CUSTOM_INCLUDE  "esp_timer.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (esp_timer_get_time() / 1000)

/* Enable/disable the built-in log system */
#define LV_USE_LOG              1
#define LV_LOG_LEVEL            LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF           1

/* Enable support for touchscreen or pointer input */
#define LV_USE_INDEV            1

/*=====================
 *   HAL SETTINGS
 *====================*/
#define LV_USE_DRAW_SW          1
#define LV_USE_DRAW_DSC_CACHE   1

/*=====================
 *  Fonts
 *====================*/
#define LV_FONT_MONTSERRAT_12   1
#define LV_FONT_MONTSERRAT_16   1
#define LV_FONT_MONTSERRAT_20   1
#define LV_FONT_MONTSERRAT_28   1

#define LV_USE_FONT_PLACEHOLDER 1

/*===================
 *  THEMES
 *===================*/
#define LV_USE_THEME_DEFAULT    1
#define LV_THEME_DEFAULT_DARK   0
#define LV_THEME_DEFAULT_GROW   1
#define LV_THEME_DEFAULT_FONT   &lv_font_montserrat_16

/*====================
 *  Widgets
 *====================*/
#define LV_USE_ARC              1
#define LV_USE_BAR              1
#define LV_USE_BTN              1
#define LV_USE_LABEL            1
#define LV_USE_SLIDER           1
#define LV_USE_SWITCH           1
#define LV_USE_TEXTAREA         1

/*====================
 *  Others
 *====================*/
#define LV_USE_ANIMATION        1
#define LV_USE_IMG              1
#define LV_USE_LINE             1
#define LV_USE_OBJ              1
#define LV_USE_GRID             1

/* Enable if using XPT2046 touch */
#define LV_USE_USER_DATA        1

#endif /* LV_CONF_H */
