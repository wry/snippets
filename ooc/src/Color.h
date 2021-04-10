#ifndef COLOR_H
#define COLOR_H

typedef struct Color
{
    char name[1<<8];

    struct Color *(*init)(struct Color *self);
    void (*deinit)(struct Color *self);

    void (*set_name)(struct Color *self, char *name);
} Color;

Color *Color_alloc(Color **self);
void Color_dealloc(Color **self);

#endif /* ifndef COLOR_H */
