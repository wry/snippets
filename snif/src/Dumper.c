//
//  Dumper.c
//  snif
//
// Copyright (c) 2024 wry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Dumper.h"

#include <inttypes.h>
#include <stdio.h>

#define FMT_COL_DEF "\033[0m"
#define FMT_COL_HI "\033[38;5;%dm"

void Dumper_dumpPacket(Dumper *self, int64_t ts, const uint8_t *packet, const uint16_t packetLength)
{
    switch (self->outputFormat)
    {
        case DumperOutputFormatColor:
            printf(FMT_COL_DEF "%" PRId64 "|%04x|", ts, packetLength);
            break;
        case DumperOutputFormatAscii:
        case DumperOutputFormatPlain:
            printf("%" PRId64 "|%04x|", ts, packetLength);
            break;
    }

    for (size_t i=0; i < packetLength; i++)
    {
        uint8_t b = packet[i];
        switch (self->outputFormat)
        {
            case DumperOutputFormatColor:
                printf(FMT_COL_HI "%02x", b, b); // color code by byte
                // u_char h = (b >> 4) & 0xf;
                // u_char l = (b >> 0) & 0xf;
                // printf(FMT_COL_HI "%x", h, h); // color code by nibble
                // printf(FMT_COL_HI "%x", l, l); // color code by nibble
                break;
            case DumperOutputFormatPlain:
                printf("%02x", b);
                break;
            case DumperOutputFormatAscii:
                printf("%c", b > 0x1f && b < 0x7f ? b : '.');
                break;
        }
    }

    switch (self->outputFormat)
    {
        case DumperOutputFormatColor:
            printf(FMT_COL_DEF "\n");
            break;
        case DumperOutputFormatPlain:
        case DumperOutputFormatAscii:
            printf("\n");
            break;
    }
}
