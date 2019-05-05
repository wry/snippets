#include <stdint.h>
#include <unistd.h>

#ifndef IMGW
#    define IMGW 300
#endif
#ifndef IMGH
#    define IMGH 300
#endif

static uint8_t fontb[] = {
#include "font.i"
};

typedef uint8_t CType;

typedef struct {
    CType x,y,z,w;
} Vec4;

typedef struct {
    int32_t x;
    int32_t y;
} Vec2;

typedef struct {
    Vec4 pixels[IMGW*IMGH];
    int32_t width;
    int32_t height;
} Image;

static const CType ccmax = sizeof(CType) == sizeof(uint8_t) ? 255 : (CType)1;
static const CType ccmin = (CType)0;
static const uint32_t hbit = 1<<(8-1);

static void wrChar(Image *img, Vec2 * pos, uint8_t asc)
{
    const size_t a_idx = 8*(asc-0x20);
    uint8_t tmp_b = 0;
    size_t off = 0;

    for (int i=0; i<8; i++) // px rows
    {
        tmp_b = fontb[a_idx+i];
        for (int j=0; j<8; j++) // px
        {
            Vec4 *px = &img->pixels[j + off + (img->width * pos->y + pos->x)];
            px->x = px->y = px->z = (tmp_b & (hbit>>j)) != 0 ? ccmax : ccmin;
            px->w = ccmax;
        }
        off+=img->width;
    }
}

static void wrImg(Image *img, int fd)
{
    write(fd, (void *)img->pixels, img->width*img->height*sizeof(Vec4));
}

static void wrTxt(Image *img, Vec2 *pos, const char *txt)
{
    uint8_t c;
    size_t i = 0;
    while ((c = txt[i++]) != '\0' && pos->y+8 < img->height)
    {
        wrChar(img, pos, c);
        pos->x += 8;
        if (pos->x+8 >= img->width) {
            pos->y += 8;
            pos->x = 0;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) return 1;

    Image image = { 0 };
    image.width = IMGW;
    image.height = IMGH;

    Vec2 pos = { 0 };
    wrTxt(&image, &pos, argv[1]);

    wrImg(&image, 1);

    return 0;
}
