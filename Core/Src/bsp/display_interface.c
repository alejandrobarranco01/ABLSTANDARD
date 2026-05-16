#include "bsp/display_interface.h"

#include "main.h"
#include "ui/fonts.h"

#include <stdlib.h>
#include <string.h>

static uint8_t framebuffer[DISPLAY_PAGES][DISPLAY_WIDTH];

extern I2C_HandleTypeDef hi2c1;
#define DISPLAY_I2C (&hi2c1)

#define I2C_TIMEOUT 10

#define FONT_SMALL_W 5
#define FONT_SMALL_H 8
#define FONT_LARGE_W 10
#define FONT_LARGE_H 16

/////////////////////////////////////////////////
///////// PRIVATE FUNCTION DECLARATIONS /////////
/////////////////////////////////////////////////

static void ssd1306_wait_ready(void);
static int ssd1306_write_cmd(uint8_t cmd);
static int ssd1306_write_data_blocking(uint8_t* data, uint16_t len);
static int ssd1306_write_data_dma(uint8_t* data, uint16_t len);
static void dma_send_page(uint8_t page);

/////////////////////////////////////////////////
/////////// DMA STATE MACHINE VARS //////////////
/////////////////////////////////////////////////

static volatile uint8_t dma_page = 0;
static volatile uint8_t dma_busy = 0;

int display_interface_init(void)
{
    HAL_Delay(100);

    if (HAL_I2C_IsDeviceReady(DISPLAY_I2C, SSD1306_I2C_ADDR << 1, 5, 1000) != HAL_OK)
    {
        LOG_ERROR("Device was not ready");
        return -1;
    }

    ssd1306_write_cmd(SSD1306_DISPLAY_OFF);

    ssd1306_write_cmd(SSD1306_SET_DISPLAY_CLOCK_DIV);
    ssd1306_write_cmd(0xF0);

    ssd1306_write_cmd(SSD1306_SET_MULTIPLEX);
    ssd1306_write_cmd(DISPLAY_HEIGHT - 1);

    ssd1306_write_cmd(SSD1306_SET_DISPLAY_OFFSET);
    ssd1306_write_cmd(0x00);

    ssd1306_write_cmd(SSD1306_SET_START_LINE);

    ssd1306_write_cmd(SSD1306_CHARGE_PUMP);
    ssd1306_write_cmd(SSD1306_CHARGE_PUMP_ENABLE);

    ssd1306_write_cmd(SSD1306_MEMORY_MODE);
    ssd1306_write_cmd(0x00);

    ssd1306_write_cmd(SSD1306_SEG_REMAP | 0x01);

    ssd1306_write_cmd(SSD1306_COM_SCAN_DEC);

    ssd1306_write_cmd(SSD1306_SET_COM_PINS);
    ssd1306_write_cmd(0x02);

    ssd1306_write_cmd(SSD1306_SET_CONTRAST);
    ssd1306_write_cmd(0x8F);

    ssd1306_write_cmd(SSD1306_SET_PRECHARGE);
    ssd1306_write_cmd(0xF1);

    ssd1306_write_cmd(SSD1306_SET_VCOM_DETECT);
    ssd1306_write_cmd(0x40);

    ssd1306_write_cmd(SSD1306_DISPLAY_ALL_ON_RESUME);
    ssd1306_write_cmd(SSD1306_NORMAL_DISPLAY);
    ssd1306_write_cmd(0x2E);
    ssd1306_write_cmd(SSD1306_DISPLAY_ON);

    display_clear();

    for (uint8_t page = 0; page < DISPLAY_PAGES; page++)
    {
        ssd1306_write_cmd(0xB0 + page);
        ssd1306_write_cmd(SSD1306_SET_LOW_COLUMN);
        ssd1306_write_cmd(SSD1306_SET_HIGH_COLUMN);
        ssd1306_write_data_blocking(framebuffer[page], DISPLAY_WIDTH);
    }

    return 0;
}

int display_interface_deinit(void) { return ssd1306_write_cmd(SSD1306_DISPLAY_OFF); }

int display_interface_task(void) { return 0; }

void display_clear(void) { memset(framebuffer, 0, sizeof(framebuffer)); }

void display_update(void)
{
    if (dma_busy)
    {
        return;
    }

    dma_page = 0;
    dma_busy = 1;
    dma_send_page(0);
}

uint8_t display_update_is_busy(void) { return dma_busy; }

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    if (hi2c->Instance != DISPLAY_I2C->Instance)
        return;

    dma_page++;
    if (dma_page < DISPLAY_PAGES)
    {
        dma_send_page(dma_page);
    }
    else
    {
        dma_busy = 0;
    }
}

/////////////////////////////////////////////////
/////////////// DRAWING FUNCTIONS ///////////////
/////////////////////////////////////////////////

void display_draw_pixel(uint8_t x, uint8_t y, bool color)
{
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT)
        return;

    uint8_t page = y / 8;
    uint8_t bit = y % 8;

    if (color)
    {
        framebuffer[page][x] |= (1 << bit);
    }
    else
    {
        framebuffer[page][x] &= (uint8_t) ~(1U << bit);
    }
}

void display_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool color)
{
    int16_t dx = (int16_t) abs((int16_t) x1 - (int16_t) x0);
    int16_t dy = (int16_t) (-abs((int16_t) y1 - (int16_t) y0));
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx + dy;

    while ((x0 != x1) || (y0 != y1))
    {
        display_draw_pixel(x0, y0, color);

        int16_t e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 = (uint8_t) (x0 + sx);
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 = (uint8_t) (y0 + sy);
        }
    }

    display_draw_pixel(x1, y1, color);
}

void display_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool filled)
{
    if (filled)
    {
        for (uint8_t row = y; row < y + height; row++)
        {
            display_draw_line(x, row, x + width - 1, row, true);
        }
    }
    else
    {
        display_draw_line(x, y, x + width - 1, y, true);
        display_draw_line(x, y + height - 1, x + width - 1, y + height - 1, true);
        display_draw_line(x, y, x, y + height - 1, true);
        display_draw_line(x + width - 1, y, x + width - 1, y + height - 1, true);
    }
}

void display_draw_circle(uint8_t x0, uint8_t y0, uint8_t r, bool filled)
{
    int16_t x = r;
    int16_t y = 0;
    int16_t err = 0;

    while (x >= y)
    {
        if (filled)
        {
            display_draw_line(x0 - x, y0 + y, x0 + x, y0 + y, true);
            display_draw_line(x0 - x, y0 - y, x0 + x, y0 - y, true);
            display_draw_line(x0 - y, y0 + x, x0 + y, y0 + x, true);
            display_draw_line(x0 - y, y0 - x, x0 + y, y0 - x, true);
        }
        else
        {
            display_draw_pixel(x0 + x, y0 + y, true);
            display_draw_pixel(x0 + y, y0 + x, true);
            display_draw_pixel(x0 - y, y0 + x, true);
            display_draw_pixel(x0 - x, y0 + y, true);
            display_draw_pixel(x0 - x, y0 - y, true);
            display_draw_pixel(x0 - y, y0 - x, true);
            display_draw_pixel(x0 + y, y0 - x, true);
            display_draw_pixel(x0 + x, y0 - y, true);
        }
        y++;
        if (err <= 0)
            err += 2 * y + 1;
        else
        {
            x--;
            err += 2 * (y - x) + 1;
        }
    }
}

void display_draw_triangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool filled)
{
    if (!filled)
    {
        display_draw_line(x0, y0, x1, y1, true);
        display_draw_line(x1, y1, x2, y2, true);
        display_draw_line(x2, y2, x0, y0, true);
        return;
    }

    int16_t ax = x0, ay = y0;
    int16_t bx = x1, by = y1;
    int16_t cx = x2, cy = y2;

    if (ay > by)
    {
        int16_t tx = ax, ty = ay;
        ax = bx;
        ay = by;
        bx = tx;
        by = ty;
    }
    if (ay > cy)
    {
        int16_t tx = ax, ty = ay;
        ax = cx;
        ay = cy;
        cx = tx;
        cy = ty;
    }
    if (by > cy)
    {
        int16_t tx = bx, ty = by;
        bx = cx;
        by = cy;
        cx = tx;
        cy = ty;
    }

    if (ay == cy)
    {
        return;
    }

    int16_t total_h = cy - ay;

    for (int16_t row = ay; row <= cy; row++)
    {
        bool lower_half = (row > by) || (by == ay);
        int16_t seg_h = lower_half ? (cy - by) : (by - ay);
        if (seg_h == 0)
        {
            seg_h = 1;
        }

        int16_t xa = ax + (cx - ax) * (row - ay) / total_h;

        int16_t xb;
        if (lower_half)
        {
            xb = bx + (cx - bx) * (row - by) / (cy - by == 0 ? 1 : cy - by);
        }
        else
        {
            xb = ax + (bx - ax) * (row - ay) / (by - ay == 0 ? 1 : by - ay);
        }

        if (xa > xb)
        {
            int16_t t = xa;
            xa = xb;
            xb = t;
        }

        if (row >= 0 && row < DISPLAY_HEIGHT)
        {
            display_draw_line((uint8_t) xa, (uint8_t) row, (uint8_t) xb, (uint8_t) row, true);
        }
    }
}

void display_draw_char(uint8_t x, uint8_t y, char c, FontSize size, bool color)
{
    if (c < 0x20 || c > 0x7E)
    {
        c = '?';
    }

    const uint8_t* glyph = font5x8[(uint8_t) c];

    if (size == FONT_SIZE_SMALL)
    {
        for (uint8_t col = 0; col < FONT_SMALL_W; col++)
        {
            uint8_t bits = glyph[col];
            for (uint8_t row = 0; row < FONT_SMALL_H; row++)
            {
                if (bits & (1 << row))
                    display_draw_pixel(x + col, y + row, color);
                else if (!color)
                    display_draw_pixel(x + col, y + row, false);
            }
        }
    }
    else
    {
        for (uint8_t col = 0; col < FONT_SMALL_W; col++)
        {
            uint8_t bits = glyph[col];
            for (uint8_t row = 0; row < FONT_SMALL_H; row++)
            {
                bool on = (bits >> row) & 1;
                bool px = on ? color : !color;
                display_draw_pixel(x + col * 2, y + row * 2, px);
                display_draw_pixel(x + col * 2 + 1, y + row * 2, px);
                display_draw_pixel(x + col * 2, y + row * 2 + 1, px);
                display_draw_pixel(x + col * 2 + 1, y + row * 2 + 1, px);
            }
        }
    }
}

void display_draw_text(uint8_t x, uint8_t y, const char* text, FontSize size, bool color)
{
    uint8_t char_w = (size == FONT_SIZE_SMALL) ? (FONT_SMALL_W + 1) : (FONT_LARGE_W + 1);
    uint8_t cx = x;

    while (*text)
    {
        if (cx + char_w > DISPLAY_WIDTH)
        {
            break;
        }
        display_draw_char(cx, y, *text, size, color);
        cx += char_w;
        text++;
    }
}

uint8_t display_get_text_width(const char* text, FontSize size)
{
    if (!text || !*text)
    {
        return 0;
    }
    uint8_t char_w = (size == FONT_SIZE_SMALL) ? (FONT_SMALL_W + 1) : (FONT_LARGE_W + 1);
    uint8_t len = (uint8_t) strlen(text);
    return (uint8_t) (len * char_w - 1);
}

void display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t* bitmap, uint8_t width, uint8_t height, bool color)
{
    // 8 bytes per row
    uint8_t stride = (uint8_t) ((width + 7) / 8);

    for (uint8_t row = 0; row < height; row++)
    {
        for (uint8_t col = 0; col < width; col++)
        {
            uint8_t byte = bitmap[row * stride + col / 8];
            bool on = (byte >> (7 - (col % 8))) & 1;
            if (on)
            {
                display_draw_pixel(x + col, y + row, color);
            }
        }
    }
}

/////////////////////////////////////////////////
/////////////// PRIVATE FUNCTIONS ///////////////
/////////////////////////////////////////////////

static void ssd1306_wait_ready(void)
{
    while (HAL_I2C_GetState(DISPLAY_I2C) != HAL_I2C_STATE_READY)
    {
    }
}

static int ssd1306_write_cmd(uint8_t cmd)
{
    ssd1306_wait_ready();
    HAL_StatusTypeDef r = HAL_I2C_Mem_Write(DISPLAY_I2C, SSD1306_I2C_ADDR << 1, 0x00, 1, &cmd, 1, I2C_TIMEOUT);
    return (r == HAL_OK) ? 0 : -1;
}

static int ssd1306_write_data_blocking(uint8_t* data, uint16_t len)
{
    ssd1306_wait_ready();
    HAL_StatusTypeDef r = HAL_I2C_Mem_Write(DISPLAY_I2C, SSD1306_I2C_ADDR << 1, 0x40, 1, data, len, 100);
    return (r == HAL_OK) ? 0 : -1;
}

static int ssd1306_write_data_dma(uint8_t* data, uint16_t len)
{
    ssd1306_wait_ready();
    HAL_StatusTypeDef r = HAL_I2C_Mem_Write_DMA(DISPLAY_I2C, SSD1306_I2C_ADDR << 1, 0x40, 1, data, len);
    return (r == HAL_OK) ? 0 : -1;
}

static void dma_send_page(uint8_t page)
{
    ssd1306_write_cmd(0xB0 + page);
    ssd1306_write_cmd(SSD1306_SET_LOW_COLUMN);
    ssd1306_write_cmd(SSD1306_SET_HIGH_COLUMN);
    ssd1306_write_data_dma(framebuffer[page], DISPLAY_WIDTH);
}
