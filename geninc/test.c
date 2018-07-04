#include <stdint.h>
#include <stdio.h>

uint8_t bytes[]=
{
#include "bytes.i"
};

int main(void)
{
    size_t len = sizeof(bytes) / sizeof(uint8_t);
    for (size_t i=0; i < len; i++)
    {
        printf("%c", bytes[i]);
    }
    return 0;
}
