#define ILI9341_DRIVER

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_MOSI 13
// #define TFT_MISO 12
#define TFT_SCLK 14
#define TFT_CS    15  
#define TFT_DC    2     // DC/RS  //esp32上没标注
#define TFT_RST   -1
#define TFT_BL   21            // LED back-light control pin 32（原本值为32）
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

#define SPI_FREQUENCY       27000000
#define SPI_READ_FREQUENCY  20000000

// 触摸屏（XPT2046）支持（如有）
// #define TOUCH_CS     25
// #define TOUCH_CLK    33
// #define TOUCH_DIN    32
// #define TOUCH_DO     39
// #define TOUCH_IRQ    36
