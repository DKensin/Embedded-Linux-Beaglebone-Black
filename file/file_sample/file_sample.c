/* creat a text file and write string into this file */

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void main(void)
{
	int fd;

	fd = open("mytext", O_CREAT | O_WRONLY);
	write(fd, "Hello world\n", strlen("Hello world\n"));

	close(fd);
}
