/* creat a text file and write string into this file */

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void main(void)
{
	int fd;
	int fd1;

	fd = open("mytext1", O_CREAT | O_WRONLY);
	fd1 = open("mytext2", O_CREAT | O_WRONLY);
	printf("fd = %d, fd1 = %d", fd, fd1);
	write(fd, "Hello world\n", strlen("Hello world\n"));

	close(fd);
}
