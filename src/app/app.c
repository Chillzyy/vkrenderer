#include "app/app.h"

app app_init(bool debugging)
{
    app a = {0};
    a.title = "Vulkan Renderer";
    a.vk = vk_context_init(a.title, debugging);
    return a;
}

void app_run(app *a)
{
    vk_context_mainloop(&a->vk);
}

void app_destroy(app *a, bool debugging)
{
    vk_context_destroy(&a->vk, debugging);
}
