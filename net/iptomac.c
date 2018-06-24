// linux code

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*  arp_flags and at_flags field values */
#define	ATF_INUSE	0x01	/* entry in use */
#define ATF_COM		0x02	/* completed entry (enaddr valid) */
#define	ATF_PERM	0x04	/* permanent entry */
#define	ATF_PUBL	0x08	/* publish entry (respond for other host) */
#define	ATF_USETRAILERS	0x10	/* has requested trailers */
#define	ATF_PROXY	0x20	/* Do PROXY arp */

int amain(void)
{
        struct sockaddr_in sin = { 0 };
        struct arpreq myarp = { { 0 } };
        int sockfd;

        sin.sin_family = AF_INET;
        inet_aton("192.168.131.103", &sin.sin_addr);

        memcpy(&myarp.arp_pa, &sin, sizeof myarp.arp_pa);
        strcpy(myarp.arp_dev, "em1");

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
                perror("socket");
                return EXIT_FAILURE;
        }

        if (ioctl(sockfd, SIOCGARP, &myarp) == -1) {
                perror("ioctl");
                return EXIT_FAILURE;
        }

        /* do something with ioctl() results */

        return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

    int s = 0;
    struct arpreq arpreq = {0};
    struct sockaddr_in *sin = NULL;
    unsigned char *eap = NULL;

    if (argc < 3)
    {
        puts("./a.out <if> <ip>");
        exit(0);
    }

    memset(&arpreq, 0, sizeof(arpreq));

    strcpy(arpreq.arp_dev, argv[1]);

    sin = (struct sockaddr_in *) &arpreq.arp_pa;
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = inet_addr(argv[2]);

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
    	perror("socket");
    	exit(0);
    }
    if (ioctl(s, SIOCGARP, &arpreq) < 0) {
    	perror("ioctl");
	close(s);
    	exit(0);
    }
    printf("IP address:       %s\n", inet_ntoa(sin->sin_addr));

    if (arpreq.arp_flags & ATF_COM) {
    	eap = (unsigned char *) &arpreq.arp_ha.sa_data[0];
    	printf("Ethernet address: %02X:%02X:%02X:%02X:%02X:%02X",
    	    	eap[0], eap[1], eap[2], eap[3], eap[4], eap[5]);
    	if (arpreq.arp_flags & ATF_PERM) printf(" PERM");
    	if (arpreq.arp_flags & ATF_PUBL) printf(" PUBLISHED");
    	if (arpreq.arp_flags & ATF_USETRAILERS) printf(" TRAILERS");
    	if (arpreq.arp_flags & ATF_PROXY) printf(" PROXY");
    	printf("\n");
    } else {
    	printf("*** INCOMPLETE ***\n");
    }
    close(s);
    exit(0);
}
