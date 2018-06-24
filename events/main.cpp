#include <iostream>
#include <memory>
#include <cstdlib>
#include <stdio.h>
#include <fcntl.h>
#include <cstdint>

#include <linux/input.h>

//#define FILE_PATH "/sys/devices/virtual/input/input3"
#define FILE_PATH "/dev/input/event0"

int send_ev(size_t fd, uint16_t type, uint16_t code, int32_t value);

int main(int argc, char *argv[])
{
  size_t version = 0;
  size_t fd = 0;

  if((fd = open(FILE_PATH, O_RDWR|O_NDELAY)) < 0)
  {
    perror("open");
    return 1;
  }

  if (ioctl(fd, EVIOCGVERSION, &version)) {
    perror("ioctl");
    return 1;
  }

  printf("evdev driver version is %d.%d.%d\n",
    version >> 16, (version >> 8) & 0xff,
    version & 0xff);

  send_ev(fd, EV_KEY, 116, 1);
  send_ev(fd, 0, 0, 0); // could use input_sync

  // to emulate long press one should sleep() here

  send_ev(fd, EV_KEY, 116, 0);
  send_ev(fd, 0, 0, 0); // could use input_sync

  close(fd);

  return 0;
}

ssize_t send_ev(size_t fd, uint16_t type, uint16_t code, int32_t value)
{
  struct input_event event = {{ 0 }};
  ssize_t len = 0;

  event.type = type;
  event.code = code;
  event.value = value;

  if (len = write(fd, &event, sizeof(event)) < 0)
  {
    perror("write");
  }

  return len;
}

