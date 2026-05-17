#include "ui.h"

#include "main.h"
#include "home_page.h"
#include "utils/time_util.h"

#include <string.h>

/////////////////////////////////////////////////
//////////////// CONFIG /////////////////////////
/////////////////////////////////////////////////

#define DISPLAY_UPDATE_TIMEOUT_MS 10U
#define UI_REFRESH_RATE_HZ 10U
#define UI_REFRESH_PERIOD_MS (1000U / UI_REFRESH_RATE_HZ)

/////////////////////////////////////////////////
///////// PRIVATE FUNCTION DECLARATIONS /////////
/////////////////////////////////////////////////

static bool page_is_valid(Page page);

static Page current_page;
static bool initialized = false;
static bool dirty = false;
static timer_t refresh_timer;

int ui_init(void)
{
    if (display_interface_init())
    {
        return -1;
    }

    memset(&current_page, 0, sizeof(Page));

    initialized = true;
    dirty = false;

    timer_start(&refresh_timer, UI_REFRESH_PERIOD_MS);

    ui_set_page(home_page);

    return 0;
}

int ui_deinit(void)
{
    if (!initialized)
    {
        return -1;
    }

    if (page_is_valid(current_page) && current_page.deinit != NULL)
    {
        current_page.deinit();
    }

    display_interface_deinit();

    memset(&current_page, 0, sizeof(Page));

    initialized = false;

    LOG_INFO("Denitialized UI");

    return 0;
}

int ui_task(void)
{
    if (!initialized)
    {
        return -1;
    }

    if (timer_is_done(&refresh_timer))
    {
        dirty = true;
        timer_restart(&refresh_timer);
    }

    if (dirty)
    {
        ui_render();
        ui_update();
        dirty = false;
    }

    return 0;
}

void ui_set_page(Page page)
{
    if (!initialized || !page_is_valid(page))
        return;

    if (page_is_valid(current_page) && current_page.deinit != NULL)
    {
        current_page.deinit();
    }

    current_page = page;

    if (current_page.init != NULL)
    {
        current_page.init();
    }

    dirty = true;
}

void ui_handle_event(UI_EVENT_E event)
{
    if (!initialized || !page_is_valid(current_page))
    {
        return;
    }

    if (current_page.handle_event != NULL)
    {
        current_page.handle_event(event);
    }

    dirty = true;
}

void ui_render(void)
{
    if (!initialized || !page_is_valid(current_page))
    {
        return;
    }

    display_clear();
    current_page.render();
}

void ui_update(void)
{
    timer_t timeout;
    timer_start(&timeout, DISPLAY_UPDATE_TIMEOUT_MS);

    while (display_update_is_busy())
    {
        if (timer_is_done(&timeout))
        {
            LOG_WARNING("Display update timed out waiting for busy flag");
            return;
        }
    }

    display_update();
}

void ui_navigate_to(const Page* page)
{
    if (page == NULL)
    {
        return;
    }
    ui_set_page(*page);
}

/////////////////////////////////////////////////
/////////////// PRIVATE FUNCTIONS ///////////////
/////////////////////////////////////////////////

static bool page_is_valid(Page page) { return page.render != NULL; }
