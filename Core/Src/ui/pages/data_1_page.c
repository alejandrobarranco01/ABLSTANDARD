#include "data_1_page.h"

#include "main.h"
#include "ui.h"
#include "home_page.h"
#include "data_manager.h"

#include <stdio.h>

/////////////////////////////////////////////////
//////////////// RENDER CONFIG //////////////////
/////////////////////////////////////////////////

#define PAGE_TITLE "Data 1"
#define TITLE_Y 0
#define DIVIDER_Y 9
#define VALUE_Y 20
#define LABEL_X 0

/////////////////////////////////////////////////
///////// PRIVATE FUNCTION DECLARATIONS /////////
/////////////////////////////////////////////////

static void data_1_page_init(void);
static void data_1_page_deinit(void);
static void data_1_page_render(void);
static void data_1_page_handle_event(UI_EVENT_E event);

const Page data_1_page = {
    .init = data_1_page_init,
    .deinit = data_1_page_deinit,
    .render = data_1_page_render,
    .handle_event = data_1_page_handle_event,
};

/////////////////////////////////////////////////
/////////////// PAGE FUNCTIONS //////////////////
/////////////////////////////////////////////////

static void data_1_page_init(void) {}

static void data_1_page_deinit(void) {}

static void data_1_page_render(void)
{
    // Title
    display_draw_text(0, TITLE_Y, PAGE_TITLE, FONT_SIZE_SMALL, true);
    display_draw_line(0, DIVIDER_Y, DISPLAY_WIDTH_PX, DIVIDER_Y, true);

    char value_str[32];

    float value;
    data_manager_get_data(&value);

    snprintf(value_str, sizeof(value_str), "Value: %.2f", (double) value);
    display_draw_text(LABEL_X, VALUE_Y, value_str, FONT_SIZE_SMALL, true);
}

static void data_1_page_handle_event(UI_EVENT_E event)
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
