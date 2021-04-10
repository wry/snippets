#include "Color.h"
#include "Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

static Color *Color_init(Color *self);
static void Color_deinit(Color *self);
static void Color_set_name(Color *self, char *name);

Color *Color_alloc(Color **self)
{
    *self = calloc(1, sizeof(Color));

    (*self)->init = Color_init;
    (*self)->deinit = Color_deinit;

    (*self)->set_name = Color_set_name;
    
    return *self;
}

void Color_dealloc(Color **self)
{
    Color_deinit(*self);
    free(*self);
    *self = NULL;
}

Color *Color_init(Color *self)
{
    bzero(self->name, sizeof(self->name));
    return self;
}

void Color_deinit(Color *self)
{
}

void Color_set_name(Color *self, char *name)
{
    strncpy(self->name, name, sizeof(self->name) - 1);
}
