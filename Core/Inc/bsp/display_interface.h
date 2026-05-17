#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

#include <stdbool.h>
#include <stdint.h>

// Display dimensions for DFR0648 (128x32)
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32
#define DISPLAY_PAGES 4 // 32 / 8 = 4 pages
#define DISPLAY_WIDTH_PX (DISPLAY_WIDTH - 1)

////////////////////////////////
/////////SSD1306 Macros/////////
////////////////////////////////

// I2C Address for DFR0648
#define SSD1306_I2C_ADDR 0x3C

// Fundamental Commands
#define SSD1306_SET_CONTRAST 0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME 0xA4
#define SSD1306_DISPLAY_ALL_ON 0xA5
#define SSD1306_NORMAL_DISPLAY 0xA6
#define SSD1306_INVERT_DISPLAY 0xA7
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF
#define SSD1306_SET_DISPLAY_OFFSET 0xD3
#define SSD1306_SET_COM_PINS 0xDA
#define SSD1306_SET_VCOM_DETECT 0xDB
#define SSD1306_SET_DISPLAY_CLOCK_DIV 0xD5
#define SSD1306_SET_PRECHARGE 0xD9
#define SSD1306_SET_MULTIPLEX 0xA8
#define SSD1306_SET_LOW_COLUMN 0x00
#define SSD1306_SET_HIGH_COLUMN 0x10
#define SSD1306_SET_START_LINE 0x40
#define SSD1306_MEMORY_MODE 0x20
#define SSD1306_COLUMN_ADDR 0x21
#define SSD1306_PAGE_ADDR 0x22
#define SSD1306_COM_SCAN_INC 0xC0
#define SSD1306_COM_SCAN_DEC 0xC8
#define SSD1306_SEG_REMAP 0xA0
#define SSD1306_CHARGE_PUMP 0x8D

// Scrolling Commands
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3

// Hardware Configuration
#define SSD1306_SET_COM_PINS_CFG 0x02

// Charge Pump
#define SSD1306_CHARGE_PUMP_ENABLE 0x14
#define SSD1306_CHARGE_PUMP_DISABLE 0x10

// Addressing Modes
#define HORIZONTAL_ADDRESSING_MODE 0x00
#define VERTICAL_ADDRESSING_MODE 0x01
#define PAGE_ADDRESSING_MODE 0x02

typedef enum
{
    FONT_SIZE_SMALL = 0,
    FONT_SIZE_LARGE = 1,
} FontSize;

int display_interface_init(void);
int display_interface_deinit(void);
int display_interface_task(void);
void display_draw_pixel(uint8_t x, uint8_t y, bool color);
void display_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool color);
void display_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool filled);
void display_draw_circle(uint8_t x0, uint8_t y0, uint8_t r, bool filled);
void display_draw_triangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool filled);
void display_draw_char(uint8_t x, uint8_t y, char c, FontSize size, bool color);
void display_draw_text(uint8_t x, uint8_t y, const char* text, FontSize size, bool color);
uint8_t display_get_text_width(const char* text, FontSize size);
void display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t* bitmap, uint8_t width, uint8_t height, bool color);
void display_clear(void);
void display_update(void);
uint8_t display_update_is_busy(void);

#endif // DISPLAY_INTERFACE_H
