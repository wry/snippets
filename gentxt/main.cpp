#include <cstdint>
#include <unistd.h>
#include <stdlib.h>

static uint8_t fontb[] = {
#include "font.i"
};

typedef struct {
  float x,y,z,w;
} Vec4;

typedef struct {
  Vec4 *pixels;
  size_t width;
  size_t height;
} Image;

static void wrChar(Image &img, Vec4 pos, uint8_t asc)
{
  uint8_t tmp_b = 0;
  float tmp_f = 0.0f;
  const size_t a_idx = 8*(asc-0x20);
  size_t off = 0;
  for (int i=0; i<8; i++) // px rows
  {
    tmp_b = fontb[a_idx+i];
    for (int j=0; j<8; j++) // px
    {
      tmp_f = (tmp_b & (0x80>>j)) != 0 ? 255.0f : 0.0f;
      Vec4 &px = img.pixels[j + off + ((size_t)img.width * (size_t)pos.y + (size_t)pos.x)];
      px.x = px.y = px.z = tmp_f;
      px.w = 1.0f;
    }
    off+=img.width;
  }
}

static void wrImg(Image &img, int fd)
{
  write(1, ((void *)img.pixels), img.width*img.height*sizeof(Vec4));
}

static void wrTxt(Image &img, Vec4 &pos, const char *txt)
{
  char c;
  size_t i = 0;
  while ((c = txt[i++]) != '\0')
  {
    wrChar(img, pos, c);
    pos.x += 8.0f;
  }
}

int main(int argc, char *argv[])
{
  Image image;

  image.pixels = (Vec4 *)calloc(300*300, sizeof(Vec4));
  image.width = 300;
  image.height = 300;

  Vec4 pos { 0.0f, 0.0f, 0.0f, 0.0f };
  wrTxt(image, pos, argv[0]);

  wrImg(image, 1);

  free(image.pixels);

  return 0;
}
