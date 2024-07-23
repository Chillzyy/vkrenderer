#include "app/app.h"

static bool check_build_type(int argc, char **argv)
{
    if ((argc > 1) && (strcmp(argv[1], "-Debugging") == 0))
    {
        return true;
    }

    return false;
}

int main(int argc, char **argv)
{
    bool debugging = check_build_type(argc, argv);

    app a = app_init(debugging);

    app_run(&a);
    app_destroy(&a, debugging);

    return 0;
}
