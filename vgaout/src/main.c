#include <stdio.h>

// https://upload.wikimedia.org/wikipedia/commons/4/49/VGA_palette.svg
// src="static unsigned char vgapal[] = {\n";document.querySelectorAll('rect').forEach((rect)=>{src+="    "+rect.style.fill.replace(/[^\d\,]+/g,"")+",\n";});src+="};";

#include "vgapalette.h"

static size_t rgb2vgargb(unsigned char rgb[3], unsigned char vgargb[3])
{
    int d = 65535;
    int tmp[3] = {0};
    int tmpl2 = 0.0f;

    size_t idx = 0;
    for (size_t i = 0; i < sizeof(vgapal); i += 3)
    {
        tmp[0] = (int)rgb[0] - (int)vgapal[i + 0];
        tmp[1] = (int)rgb[1] - (int)vgapal[i + 1];
        tmp[2] = (int)rgb[2] - (int)vgapal[i + 2];
        tmpl2 = tmp[0] * tmp[0] + tmp[1] * tmp[1] + tmp[2] * tmp[2];

        if (tmpl2 < d)
        {
            d = tmpl2;
            idx = i;
        }
    }

    vgargb[0] = vgapal[idx + 0];
    vgargb[1] = vgapal[idx + 1];
    vgargb[2] = vgapal[idx + 2];

    return idx / 3;
}

static int rgb2vga(unsigned char rgb[3], size_t idx)
{
    size_t len = sizeof(vgapal) / 3;
    for (int i = 0; i < len; i++)
    {
        if (vgapal[0] == rgb[0] && vgapal[1] == rgb[1] && vgapal[2] == rgb[2])
        {
            return idx = i;
        }
    }
    return -1;
}

static int vga2rgb(size_t idx, unsigned char rgb[3])
{
    size_t len = sizeof(vgapal) / 3;
    if (idx > len)
    {
        return -1;
    }
    size_t idx3 = idx * 3;
    rgb[0] = vgapal[idx3 + 0];
    rgb[1] = vgapal[idx3 + 1];
    rgb[2] = vgapal[idx3 + 2];
    return 0;
}

int main()
{
    unsigned char rgb[3] = {0, 0, 170};
    unsigned char rgb2[3] = {0, 0, 0};
    size_t vga = 6;

    vga2rgb(vga, rgb);
    printf("index %zu : %d,%d,%d\n", vga, rgb[0], rgb[1], rgb[2]);

    rgb2vga(rgb, vga);
    printf("index %zu : %d,%d,%d\n", vga, rgb[0], rgb[1], rgb[2]);

    rgb[0] = 237, rgb[1] = 5, rgb[2] = 64;
    vga = rgb2vgargb(rgb, rgb2);
    printf("rgb(%d,%d,%d) -> rgb(%d,%d,%d), vga : %zu\n", rgb[0], rgb[1], rgb[2], rgb2[0], rgb2[1], rgb2[2], vga);

    return 0;
}
