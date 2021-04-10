#include "Ac.h"
#include "Color.h"
#include "Log.h"

#include <stdio.h>

int main()
{
    // create color instance
    AC_ALLOC(Color, color);

    color->init(color);
    color->set_name(color, "name");

    // create log instance
    AC_ALLOC(Log, log);

    log->init(log);

    // log color name
    LOG_D(log, "color name is '%s'", color->name);

    // trace log
    LOG_T(log);

    return 0;
}
