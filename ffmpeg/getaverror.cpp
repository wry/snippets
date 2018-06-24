// eg.: ./0 -541478725

#include <stdio.h>

extern "C" {
  #include <libavutil/avutil.h>
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("./0 <errcode>\n");
    return EXIT_FAILURE;
  }

  int errcode = atoi(argv[1]);
  constexpr size_t buflen = 2048;
  char buf[buflen] = {0};
  if(av_strerror(errcode, buf, buflen-1) == 0)
  {
    printf("%s\n", buf);
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
