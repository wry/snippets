//
//  main.cpp
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
#include "Sniffer.h"

#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

static Sniffer sniffer = {0};
static Dumper dumper = {0};

static void sighandler(int signo)
{
    printf("Signal received : %d", signo);

    switch (signo)
    {
    case SIGINT:
    case SIGTERM:
    case SIGHUP:
        Sniffer_stop(&sniffer);
        break;
    default:
        break;
    }
}

static void sigsettraps(void)
{
    {
        struct sigaction actint;
        actint.sa_handler = sighandler;
        sigemptyset(&actint.sa_mask);
        actint.sa_flags = SA_RESETHAND;

        if (sigaction(SIGINT, &actint, 0) != 0)
        {
            printf("Unable to install INT signal handler!");
        }
        if (sigaction(SIGTERM, &actint, 0) != 0)
        {
            printf("Unable to install INT signal handler!");
        }
        if (sigaction(SIGHUP, &actint, 0) != 0)
        {
            printf("Unable to install INT signal handler!");
        }
    }
}

static void handleCapturedTCPPacket(Sniffer *sniffer, const struct pcap_pkthdr *phdr, const struct ip *iphdr, const struct tcphdr *tcphdr, const u_char *packet)
{
    (void)sniffer;

    const uint16_t packetLength = ntohs(iphdr->ip_len) - (uint16_t)iphdr->ip_hl * 4 - (uint16_t)tcphdr->th_off * 4;

    Dumper_dumpPacket(&dumper, phdr->ts.tv_sec, packet, packetLength);
}

static void handleCapturedUDPPacket(Sniffer *sniffer, const struct pcap_pkthdr *phdr, const struct ip *iphdr, const struct udphdr *udphdr, const u_char *packet)
{
    (void)sniffer;
    (void)iphdr;

    const uint16_t packetLength = ntohs(udphdr->uh_ulen) - sizeof(struct udphdr);

    Dumper_dumpPacket(&dumper, phdr->ts.tv_sec, packet, packetLength);
}

static void show_help(void)
{
    printf("$0 [-i <if> | -r <path>] [-f <filter>] [-t <0|1|2>]\n");
}

int main(int argc, char *argv[])
{
    char *src = NULL;
    char *filter = NULL;
    uint8_t srcIsPath = 0;

    int c;
    while ((c = getopt(argc, argv, "i:r:f:t:")) != -1)
    {
        switch (c)
        {
        case 'i':
            src = optarg;
            srcIsPath = 0;
            break;
        case 'r':
            src = optarg;
            srcIsPath = 1;
            break;
        case 'f':
            filter = optarg;
            break;
        case 't':
            dumper.outputFormat = (DumperOutputFormat)MIN(atoi(optarg), (int)DumperOutputFormatAscii);
            break;
        case '?':
        default:
            break;
        }
    }

    if (!src)
    {
        show_help();
        return EXIT_FAILURE;
    }

    sigsettraps();

    Sniffer_init(&sniffer, src, srcIsPath, filter);
    sniffer.tcpPkHandler = handleCapturedTCPPacket;
    sniffer.udpPkHandler = handleCapturedUDPPacket;
    Sniffer_start(&sniffer);
    Sniffer_destroy(&sniffer);

    return EXIT_SUCCESS;
}
