// https://wiki.multimedia.cx/index.php/YUV4MPEG2
// ./vidgen | ffmpeg -y -f rawvideo -pixel_format yuv444p -video_size 256x256 -i pipe:0 -f mpegts udp://127.0.0.1:23000
// ./vidgen | ffmpeg -y -i pipe:0 -f mpegts udp://127.0.0.1:23000
// ./vidgen | ffmpeg -y -i pipe:0 -f mpegts bla.avi
// vlc udp://@:23000
// ./vidgen | ffplay -i pipe:0

#define PI 3.14159265359

static int write(int fd, const void * buf, unsigned long buflen)
{
  int ret;
  asm volatile (
    "movq $0x02000004, %%rax\n" // 4
    "syscall\n"
    : "=a" (ret)
    : "D" (fd), "S" (buf), "d" (buflen));
  return ret;
}

static float fsin(float r)
{
  float ret;
  asm ("fsin" : "=t" (ret) : "0" (r));
  return ret;
}

static float fcos(float r)
{
  float ret;
  asm ("fcos" : "=t" (ret) : "0" (r));
  return ret;
}

static float fsqrt(float v)
{
  float ret;
  asm ("fsqrt" : "=t" (ret) : "0" (v));
  return ret;
}

static float fabs(float v)
{
  float ret;
  asm ("fabs" : "=t" (ret) : "0" (v));
  return ret;
}

static double fpatan(float y, float x)
{ 
  float ret; 
  asm (
     "fpatan" 
     : "=t" (ret)
     : "0" (x), "u" (y)
     );   
  return ret; 
}

static void rgb2yuv444(unsigned char rgb[3], unsigned char yuv[3])
{
  yuv[0] = ( (  66 * rgb[0] + 129 * rgb[1] +  25 * rgb[2] + 128) >> 8) + 16;
  yuv[1] = ( ( -38 * rgb[0] -  74 * rgb[1] + 112 * rgb[2] + 128) >> 8) + 128;
  yuv[2] = ( ( 112 * rgb[0] -  94 * rgb[1] -  18 * rgb[2] + 128) >> 8) + 128;
}

static float vec2length(float v[2])
{
  return fsqrt(v[0] * v[0] + v[1] * v[1]);
}

static void vec3mix(float dst[3], float v1[3], float v2[3], float a)
{
  dst[0] = fsqrt(v1[0] * v1[0] * (1.0f - a) + v2[0] * v2[0] * a);
  dst[1] = fsqrt(v1[1] * v1[1] * (1.0f - a) + v2[1] * v2[1] * a);
  dst[2] = fsqrt(v1[2] * v1[2] * (1.0f - a) + v2[2] * v2[2] * a);
}

void start(void)
{
  const unsigned short w = 256, h = 256;
  const unsigned long res = w * h;

  unsigned char yuv[3];
  unsigned char buf[res * 3];

  int f = 0;

  float t, r, uv[2];

  while (1)
  {
    if (f == 0)
    {
      write(1, "YUV4MPEG2 W256 H256 F30:1 Ip A0:0 C444 XYSCSS=444\n", 50);
    }
    write(1, "FRAME\n", 6);

    t = (float)f / 30.0f;

    float col[3];
    float col2[3];

    unsigned char coli[3];
    coli[0] = coli[1] = coli[2] = 0;

    for (int y=0; y < h; y++)
    {
      for (int x=0; x < w; x++)
      {
        uv[0] = -1.0 + 2.0 * ((float)x / (float)w);
        uv[1] = -1.0 + 2.0 * ((float)y / (float)h);

        col[0] = 0.9f, col[1] = 0.9f, col[2] = 0.9f;

        col2[0] = 0.9f, col2[1] = 0.3f, col2[2] = 0.3f;
        r = vec2length(uv) < 0.5f ? 1.0f : 0.0f;
        r *= PI*fsin(t) < fpatan(uv[1], uv[0]) ? 0.0f : 1.0f;
        r *= vec2length(uv) > 0.25f ? 1.0f : 0.0f;
        vec3mix(col, col, col2, r);

        col2[0] = 0.3f, col2[1] = 0.8f, col2[2] = 0.3f;
        r = vec2length(uv) < 0.35f ? 1.0f : 0.0f;;
        r *= fpatan(uv[1], uv[0]) > PI*fcos(t) ? 1.0f : 0.0f;
        vec3mix(col, col, col2, r);

        coli[0] = (unsigned char)(256.0f * col[0]);
        coli[1] = (unsigned char)(256.0f * col[1]);
        coli[2] = (unsigned char)(256.0f * col[2]);

        rgb2yuv444(coli, yuv);

        buf[y * h + x + 0] = yuv[0];
        buf[y * h + x + res] = yuv[1];
        buf[y * h + x + res * 2] = yuv[2];
      }
    }

    write(1, buf, res * 3);

    f++;
  }
}
