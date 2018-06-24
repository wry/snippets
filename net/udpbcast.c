// unix/linux udp broadcast sender/listener

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOG_TEXT(...) printf(__VA_ARGS__)

typedef struct
{
	const char * addr;
	int port;
	const char * msg;
} Conf;

static int _send(Conf * conf);
static int _listen(Conf * conf);

int main(int argc, char * argv[])
{
	Conf conf;

	if (argc > 4 && strcmp(argv[1], "s") == 0)
	{
		conf.port = atoi(argv[2]);
		conf.addr = argv[3];
		conf.msg = argv[4];
		return _send(&conf);
	}
	else if (argc > 2 && strcmp(argv[1], "l") == 0)
	{
		conf.port = atoi(argv[2]);
		conf.addr = NULL;
		conf.msg = NULL;
		return _listen(&conf);
	}
	else
	{
		LOG_TEXT("./0 [s|l] <port> [<addr> <message>] # 's' send, 'l' listen\n");
	}

	return 1;
}

int _send(Conf * conf)
{
	int sd;
	if ((sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("socket");
		return 1;
	}

	int bc = 1;
	if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &bc, sizeof(bc)) < 0)
	{
		perror("setsockopt");
		close(sd);
		return 1;
	}

	struct sockaddr_in broadcastAddr;
	memset(&broadcastAddr, 0, sizeof broadcastAddr);
	broadcastAddr.sin_family = AF_INET;
	inet_pton(AF_INET, conf->addr, &broadcastAddr.sin_addr);
	broadcastAddr.sin_port = htons(conf->port);

	if (sendto(sd, conf->msg, strlen(conf->msg), 0, (struct sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) < 0)
	{
		perror("sendto");
		close(sd);
		return 1;
	}

	LOG_TEXT("message sent.\n");

	close(sd);

	return 0;

}

int _listen(Conf * conf)
{
	int sd;
	struct sockaddr_in si_me, client_address;

	if ((sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("socket");
		return 1;
	}

	int bc = 1;
	if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &bc, sizeof(bc)) < 0)
	{
		perror("setsockopt");
		close(sd);
		return 1;
	}

	memset(&si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	si_me.sin_port = htons(conf->port);

	if (bind(sd, (struct sockaddr *)&si_me, sizeof(struct sockaddr)) < 0)
	{
		perror("bind->");
		close(sd);
		return 1;
	}

	LOG_TEXT("listening..\n");

	while (1)
	{
		char buf[64] = { 0 };
		socklen_t client_address_len = sizeof(client_address);

		recvfrom(sd, buf, sizeof(buf)-1, 0, (struct sockaddr *)&client_address, &client_address_len);

		LOG_TEXT("recv: %s\n", buf);
	}

	close(sd);

	return 0;
}

