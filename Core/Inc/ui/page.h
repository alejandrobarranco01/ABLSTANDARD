#ifndef PAGE_H
#define PAGE_H

#include "ui/ui.h"
#include "ui/ui_types.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct Page Page;

typedef struct Page
{
    void (*init)(void);
    void (*deinit)(void);
    void (*render)(void);
    void (*handle_event)(UI_EVENT_E event);
} Page;

#endif // PAGE_H
