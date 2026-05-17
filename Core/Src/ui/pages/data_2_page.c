#include "data_2_page.h"

#include "main.h"
#include "ui.h"
#include "home_page.h"
#include "data_manager.h"

#include <stdio.h>

#define PAGE_TITLE "Data 2"
#define TITLE_Y 0
#define DIVIDER_Y 9
#define LABEL_X 0

#define TABLE_X 0
#define TABLE_Y (DIVIDER_Y + 1)
#define TABLE_W DISPLAY_WIDTH
#define TABLE_H (DISPLAY_HEIGHT - TABLE_Y)
#define CELL_W (TABLE_W / 2)
#define CELL_H (TABLE_H / 2)
#define FONT_SMALL_H_PX 8

static void data_2_page_init(void);
static void data_2_page_deinit(void);
static void data_2_page_render(void);
static void data_2_page_handle_event(UI_EVENT_E event);
static void draw_table(float value);

const Page data_2_page = {
    .init = data_2_page_init,
    .deinit = data_2_page_deinit,
    .render = data_2_page_render,
    .handle_event = data_2_page_handle_event,
};

static void data_2_page_init(void) {}

static void data_2_page_deinit(void) {}

static void data_2_page_render(void)
{
    display_draw_text(0, TITLE_Y, PAGE_TITLE, FONT_SIZE_SMALL, true);
    display_draw_line(0, DIVIDER_Y, DISPLAY_WIDTH_PX, DIVIDER_Y, true);

    float value;
    data_manager_get_data(&value);

    draw_table(value);
}

static void data_2_page_handle_event(UI_EVENT_E event)
{
    switch (event)
    {
        case LEFT_BUTTON_PRESS:
            ui_navigate_to(&home_page);
            break;

        case RIGHT_BUTTON_PRESS:
            break;

        default:
            LOG_ERROR("Unknown UI event %d", event);
            break;
    }
}

static void draw_table(float value)
{
    // Outer border
    display_draw_rect(TABLE_X, TABLE_Y, TABLE_W, TABLE_H, false);

    // Vertical divider
    display_draw_line(TABLE_X + CELL_W, TABLE_Y, TABLE_X + CELL_W, TABLE_Y + TABLE_H - 1, true);

    // Horizontal divider
    display_draw_line(TABLE_X, TABLE_Y + CELL_H, TABLE_X + TABLE_W - 1, TABLE_Y + CELL_H, true);

    // Same value in all 4 cells for now
    char value_str[16];
    snprintf(value_str, sizeof(value_str), "%.2f", (double) value);

    uint8_t text_w = display_get_text_width(value_str, FONT_SIZE_SMALL);

    for (uint8_t row = 0; row < 2; row++)
    {
        for (uint8_t col = 0; col < 2; col++)
        {
            uint8_t cell_x = TABLE_X + col * CELL_W;
            uint8_t cell_y = TABLE_Y + row * CELL_H;
            uint8_t text_x = cell_x + (CELL_W - text_w) / 2;
            uint8_t text_y = cell_y + (CELL_H - FONT_SMALL_H_PX) / 2;
            display_draw_text(text_x, text_y, value_str, FONT_SIZE_SMALL, true);
        }
    }
}
