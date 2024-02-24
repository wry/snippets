//
//  Sniffer.h
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

#ifndef __snif__Sniffer_h__
#define __snif__Sniffer_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

#include <pcap/pcap.h>

#define SNIFFER_OK 0
#define SNIFFER_ERROR 1

struct ip;
struct tcphdr;
struct udphdr;
struct Sniffer;

typedef void (SnifferIPPacketHandler)(struct Sniffer *, const struct pcap_pkthdr *, const struct ip *iphdr, const u_char *data);
typedef void (SnifferTCPPacketHandler)(struct Sniffer *, const struct pcap_pkthdr *, const struct ip *iphdr, const struct tcphdr *tcphdr, const u_char *data);
typedef void (SnifferUDPPacketHandler)(struct Sniffer *, const struct pcap_pkthdr *, const struct ip *iphdr, const struct udphdr *udphdr, const u_char *data);

typedef struct Sniffer
{
    pcap_t *handle;
    const char *src;
    uint8_t srcIsPath;
    const char *filter;
    char err[1024 + PCAP_ERRBUF_SIZE];
    int dlhdrSize;
    SnifferIPPacketHandler *ipPkHandler;
    SnifferTCPPacketHandler *tcpPkHandler;
    SnifferUDPPacketHandler *udpPkHandler;
} Sniffer;

void Sniffer_init(Sniffer *, const char *src, uint8_t srcIsPath, const char *filter);
void Sniffer_destroy(Sniffer *);

int Sniffer_start(Sniffer *);
void Sniffer_stop(Sniffer *);

#ifdef __cplusplus
}
#endif

#endif /* defined(__snif__Sniffer_h__) */
