//
//  Sniffer.c
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

#include "Sniffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#define LOG_HELPER(fmt, ...) printf(fmt "\n%s", __VA_ARGS__)
#define LOG_DEB(...) LOG_HELPER(__VA_ARGS__, "")
#define CHKERR(F, V)                                                                         \
    do {                                                                                     \
        if (V)                                                                               \
        {                                                                                    \
            memset(self->err, 0, sizeof(self->err));                                         \
            snprintf(self->err, sizeof(self->err), "%s : %s", F, pcap_geterr(self->handle)); \
            LOG_DEB("error : %s", self->err);                                                \
            pcap_close(self->handle);                                                        \
            self->handle = NULL;                                                             \
            return 1;                                                                        \
        }                                                                                    \
    } while (0);

static void handle_pcap_packet(u_char *user, const struct pcap_pkthdr *header, const u_char *packet);

void Sniffer_init(Sniffer *self, const char *src, uint8_t srcIsPath, const char *filter)
{
    self->handle = NULL;
    self->src = src;
    self->srcIsPath = srcIsPath;
    self->filter = filter;
    memset(self->err, 0, sizeof(self->err));
    self->dlhdrSize = 0;
    self->ipPkHandler = NULL;
    self->tcpPkHandler = NULL;
    self->udpPkHandler = NULL;
}

void Sniffer_destroy(Sniffer *self)
{
    if (self->handle)
    {
        pcap_close(self->handle);
        self->handle = NULL;
    }
}

int Sniffer_start(Sniffer *self)
{
    if (self->srcIsPath)
    {
        if ((self->handle = pcap_open_offline(self->src, self->err)) == NULL)
        {
            LOG_DEB("failed to initialize : %s", self->err);
            return SNIFFER_ERROR;
        }
    }
    else
    {
        if ((self->handle = pcap_create(self->src, self->err)) == NULL)
        {
            LOG_DEB("failed to initialize : %s", self->err);
            return SNIFFER_ERROR;
        }
        CHKERR("pcap_set_snaplen", pcap_set_snaplen(self->handle, 65535));
        CHKERR("pcap_set_timeout", pcap_set_timeout(self->handle, 1000));
        CHKERR("pcap_activate", pcap_activate(self->handle));
    }

    if (self->filter && strlen(self->filter) > 0)
    {
        struct bpf_program fp;
        CHKERR("pcap_compile", pcap_compile(self->handle, &fp, self->filter, 0, (bpf_u_int32)0));
        CHKERR("pcap_setfilter", pcap_setfilter(self->handle, &fp));
    }

    int linktype;
    if ((linktype = pcap_datalink(self->handle)) == PCAP_ERROR_NOT_ACTIVATED)
    {
        LOG_DEB("pcap not activated");
        return SNIFFER_ERROR;
    }
 
    switch (linktype)
    {
        case DLT_RAW:
            self->dlhdrSize = 0;
            break;
        case DLT_NULL:
            self->dlhdrSize = 4;
            break;
        case DLT_EN10MB:
            self->dlhdrSize = 14;
            break;
        case DLT_SLIP:
        case DLT_PPP:
            self->dlhdrSize = 24;
            break;
        default:
            LOG_DEB("unhandled link type : %d", linktype);
            return SNIFFER_ERROR;
    }

    CHKERR("pcap_loop", pcap_loop(self->handle, 0, handle_pcap_packet, (u_char *)self));

    pcap_close(self->handle);
    self->handle = NULL;

    return SNIFFER_OK;
}

void Sniffer_stop(Sniffer *sniffer)
{
    LOG_DEB("Stopping...");
    pcap_breakloop(sniffer->handle);
}

void handle_pcap_packet(u_char *user, const struct pcap_pkthdr *header, const u_char *packet)
{
    Sniffer *sniffer = (Sniffer*)user;

    // skip datalink hdr
    packet += sniffer->dlhdrSize;

    // get ip hdr
    const struct ip *iphdr = (struct ip*)packet;

    if (sniffer->ipPkHandler)
    {
        sniffer->ipPkHandler(sniffer, header, iphdr, packet);
    }

    // skip ip hdr
    packet += iphdr->ip_hl * 4;

    switch (iphdr->ip_p)
    {
        case IPPROTO_TCP:
        {
            struct tcphdr *tcphdr = (struct tcphdr*)packet;
            // skip tcp hdr
            packet += tcphdr->th_off * 4;
            if (sniffer->tcpPkHandler)
            {
                sniffer->tcpPkHandler(sniffer, header, iphdr, tcphdr, packet);
            }
            break;
        }
        case IPPROTO_UDP:
        {
            struct udphdr *udphdr = (struct udphdr*)packet;
            // skip udp hdr
            packet += sizeof(struct udphdr);
            if (sniffer->udpPkHandler)
            {
                sniffer->udpPkHandler(sniffer, header, iphdr, udphdr, packet);
            }
            break;
        }
        default:
        {
            //LOG_DEB("unhandled protocol : %d", (int)iphdr->ip_p);
            break;
        }
    }
}
