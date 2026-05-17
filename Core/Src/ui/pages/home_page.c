#include "home_page.h"
#include "data_1_page.h"
#include "data_2_page.h"

#include "main.h"
#include "ui.h"

/////////////////////////////////////////////////
//////////////// MENU CONFIG ////////////////////
/////////////////////////////////////////////////

typedef struct
{
    const char* label;
    const Page* target;
} menu_item_t;

static const menu_item_t menu_items[] = {
    {"Data 1", &data_1_page},
    {"Data 2", &data_2_page},
};

#define MENU_ITEM_COUNT (sizeof(menu_items) / sizeof(menu_items[0]))

/////////////////////////////////////////////////
//////////////// RENDER CONFIG //////////////////
/////////////////////////////////////////////////

#define PAGE_TITLE "Home Page"
#define TITLE_Y 0
#define DIVIDER_Y 9
#define MENU_START_Y 12
#define MENU_ROW_HEIGHT 10
#define CURSOR_X 0
#define LABEL_X 8

/////////////////////////////////////////////////
//////////////// PAGE STATE /////////////////////
/////////////////////////////////////////////////

static uint8_t selected_item = 0;

/////////////////////////////////////////////////
///////// PRIVATE FUNCTION DECLARATIONS /////////
/////////////////////////////////////////////////

static void home_page_init(void);
static void home_page_deinit(void);
static void home_page_render(void);
static void home_page_handle_event(UI_EVENT_E event);

const Page home_page = {
    .init = home_page_init,
    .deinit = home_page_deinit,
    .render = home_page_render,
    .handle_event = home_page_handle_event,
};

/////////////////////////////////////////////////
/////////////// PAGE FUNCTIONS //////////////////
/////////////////////////////////////////////////

static void home_page_init(void) { selected_item = 0; }

static void home_page_deinit(void) {}

static void home_page_render(void)
{
    // Title
    display_draw_text(0, TITLE_Y, PAGE_TITLE, FONT_SIZE_SMALL, true);

    // Divider under title
    display_draw_line(0, DIVIDER_Y, DISPLAY_WIDTH_PX, DIVIDER_Y, true);

    // Menu items
    for (uint8_t i = 0; i < MENU_ITEM_COUNT; i++)
    {
        uint8_t y = (uint8_t) (MENU_START_Y + (i * MENU_ROW_HEIGHT));

        if (i == selected_item)
        {
            display_draw_text(CURSOR_X, y, ">", FONT_SIZE_SMALL, true);
        }
        display_draw_text(LABEL_X, y, menu_items[i].label, FONT_SIZE_SMALL, true);
    }
}

static void home_page_handle_event(UI_EVENT_E event)
{
    switch (event)
    {
        case LEFT_BUTTON_PRESS:
            // Scroll down through the list (wraps to top)
            if (selected_item >= MENU_ITEM_COUNT - 1)
            {
                selected_item = 0;
            }
            else
            {
                selected_item++;
            }
            break;

        case RIGHT_BUTTON_PRESS:
            // Select current item -> navigate to its target page
            if (menu_items[selected_item].target != NULL)
            {
                ui_navigate_to(menu_items[selected_item].target);
            }
            else
            {
                LOG_WARNING("Menu item '%s' has no target page", menu_items[selected_item].label);
            }
            break;

        default:
            LOG_ERROR("Unknown UI event %d", event);
            break;
    }
}
