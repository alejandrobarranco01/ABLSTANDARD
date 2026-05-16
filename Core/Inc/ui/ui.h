#ifndef UI_H
#define UI_H

#include "display_interface.h"
#include "page.h"
#include "ui_types.h"

typedef struct Page Page;

int ui_init(void);
int ui_deinit(void);
int ui_task(void);
void ui_set_page(Page page);
void ui_handle_event(UI_EVENT_E event);
void ui_render(void);
void ui_update(void);
void ui_navigate_to(const Page* page);

#endif // UI_H
