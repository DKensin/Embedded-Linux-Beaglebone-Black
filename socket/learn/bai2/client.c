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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main()
{
	int sockfd = -1;
	struct sockaddr_in server_addr;
	char recv_buffer[8192];

	memset(recv_buffer, 0, sizeof(recv_buffer));
	memset(&server_addr, 0, sizeof(server_addr));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.100.104");
	server_addr.sin_port = htons(7003);

	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
		read(sockfd, recv_buffer, sizeof(recv_buffer)-1);
	}

	close(sockfd);

	int fd = -1;
	fd = open("data", O_WRONLY);

	if (fd < 0) {
		puts("Error while open file");
		exit(EXIT_FAILURE);
	}

	write(fd, recv_buffer, strlen(recv_buffer));

	close(fd);
}
