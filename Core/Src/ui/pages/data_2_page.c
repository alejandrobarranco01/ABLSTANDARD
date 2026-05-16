#include "data_2_page.h"

#include "main.h"
#include "ui.h"
#include "home_page.h"
// #include "data/data_manager.h"

#include <stdio.h>

#define PAGE_TITLE "Data 2"
#define TITLE_Y 0
#define DIVIDER_Y 9
#define VALUE_Y 20
#define LABEL_X 0
#define DISPLAY_WIDTH_PX 127

static void data_2_page_init(void);
static void data_2_page_deinit(void);
static void data_2_page_render(void);
static void data_2_page_handle_event(UI_EVENT_E event);

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

    char value_str[32];

    // int32_t value = data_manager_get_data(DATA_ID_2);
    int32_t value = 0;

    (void) snprintf(value_str, sizeof(value_str), "Value: %ld", (long) value);
    display_draw_text(LABEL_X, VALUE_Y, value_str, FONT_SIZE_SMALL, true);
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
