#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char buf[10] = "Hello ";
	char name[100] = { 0 };
	char newline[] = {"\n"};

	printf("Enter name: ");
	scanf("%s", name);

	int fd = -1;

	fd = open("data.txt", O_WRONLY);

	if (fd < 0)
	{
		printf("Error while open file.\n");
		exit(1);
	}
	else
	{
		write(fd, buf, strlen(buf) );
	}
	lseek(fd, 0, SEEK_END);
	write(fd, name, strlen(name) );
	lseek(fd, 0, SEEK_END);
	write(fd, newline, strlen(newline));

	close(fd);
}
