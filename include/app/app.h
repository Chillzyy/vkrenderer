#ifndef app_h
#define app_h

#include "gfx/vk/vk_context.h"

typedef struct
{
    vk_context vk;
    const char *title;
}   app;

app app_init(bool debugging);
void app_run(app *a);
void app_destroy(app *a, bool debugging);

#endif
