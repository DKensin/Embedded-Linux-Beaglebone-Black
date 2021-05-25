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
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

void main()
{
	int listenfd = -1;	/* server port for listen */
	int connfd = -1;  	/* port for communication with client */
	struct sockaddr_in server_addr;
	char send_buffer[8192];

	int data_fd = -1;
	int len = -1;

	memset(&server_addr, 0, sizeof(server_addr)); /* set to 0 */
	memset(send_buffer, 0, sizeof(send_buffer));

	listenfd = socket(AF_INET, SOCK_STREAM, 0); /* open socket */

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(60002);
	bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	listen(listenfd, 10);
	
	while(1) {
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		
		memset(send_buffer, 0, sizeof(send_buffer));

		data_fd = open("data_server", O_RDONLY);
		len = lseek(data_fd, 0, SEEK_END);
		lseek(data_fd, 0, SEEK_SET);
		read(data_fd, send_buffer, len);
		close(data_fd);

		write(connfd, send_buffer, len);

		close(connfd);	
	}

	close(listenfd);
}
