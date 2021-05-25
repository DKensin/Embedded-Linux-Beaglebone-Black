#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

void main()
{
	int listenfd = -1;
	int connfd = -1;
	struct sockaddr_in server_addr;
	char send_buffer[1024];
	time_t ticks;

	memset(&server_addr, 0, sizeof(server_addr));

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.100.104");
	server_addr.sin_port = htons(6000);
	bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	listen(listenfd, 10);
	while(1) {
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		ticks = time(NULL);
		sprintf(send_buffer, "Server reply %s", ctime(&ticks));
		write(connfd, send_buffer, strlen(send_buffer));
		close(connfd);
	}

	close(listenfd);
}
